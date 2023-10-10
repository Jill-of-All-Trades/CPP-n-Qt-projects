#include "simousolver.h"
#include <QDebug>

#include <Eigen/Dense>


// Useful resources
// 1. Eigen regression
// https://github.com/Kolkir/mlcpp/tree/master/polynomial_regression_eigen
//
//

// NOTES
/* Алгоритм Симою
 * Имеются дискретные значения времени t, с равным интервалом
 * Имеются значения величины P в соотв. с t
 * Надо будет искать площади F1, F2, F3... по формулам
 *
 * F1 находится просто и без проблем.
 * Затем нужно произвести регрессию функции f( phi ) = 1 - sigma
 * Регриссия должна быть полимиальной, и не отрицательной.
 *
 * P.S. На google drive есть colab файлик именнованный RapedDeadTurtle - это для тестов
*/

SimouSolver::SimouSolver(QList<QPointF> *points, size_t polynome_degree)
{
    this->points = points;
    this->polynome_degree = polynome_degree;
    regression_points_amount = static_cast<uint>(points->size());
}

// GET & SET

void SimouSolver::CalcDiscreteFrequency()
{
    int points_cnt = points->size();
    double sum = 0;
    for(int i=0; i < points_cnt-1; ++i) {
        QPointF p0 = (*points)[i];
        QPointF p1 = (*points)[i+1];
        sum += p1.x() - p0.x();
    }
    discrete_freq = sum / (points_cnt-1);
    qDebug() << "[SimouSolver] Discrete Frequency = " << discrete_freq;
}

double SimouSolver::GetDiscreteFrequency() const
{
    return discrete_freq;
}

void SimouSolver::SetPolynomialDegree(size_t degree)
{
    polynome_degree = degree;
}

size_t SimouSolver::GetPolynomailDegree() const
{
    return polynome_degree;
}

void SimouSolver::SetRegressionPointsAmount(uint amount)
{
    regression_points_amount = amount;
}

uint SimouSolver::GetRegressionPointsAmount() const
{
    return regression_points_amount;
}

// SOLVER

double* SimouSolver::Solve()
{
    assert(regression_points_amount > 0);
    int points_cnt = points->size();
    Eigen::VectorXd P( points_cnt );
    Eigen::VectorXd t( points_cnt );

    for(int i=0; i < points_cnt; ++i) {
        QPointF pp = (*points)[i];
        t[i] = pp.x();
        P[i] = pp.y();
    }

    Eigen::VectorXd y( points_cnt );
    Eigen::VectorXd _P0(points_cnt);
    _P0.fill(P[0]);
    y = _P0 - P;

    Eigen::VectorXd sigma( points_cnt );
    sigma = y / (P[0] - P[points_cnt-1]);

    // F1
    double sumF1 = 0;
    for(int i=0; i < points_cnt; ++i) {
        sumF1 += (1 - sigma[i]);
    }
    sumF1 -= 0.5*(1-sigma[0]);
    double F1 = discrete_freq * sumF1;

    qDebug() << "F1 = " << F1;

    // =================================
    Eigen::VectorXd phi( points_cnt );
    phi = t / F1;

    Eigen::VectorXd ones(points_cnt);
    Eigen::VectorXd f0(points_cnt);
    ones.fill(1);
    f0 = ones - sigma;

    qDebug() << "phi rows = " << phi.rows();

    // =================================
    // Regression
    // =================================
    Matrix b = regression(phi, f0);

    double phi_min = phi.minCoeff();
    double phi_max = phi.maxCoeff();
    qDebug() << phi_min << " " << phi_max;
    double d_omega = (phi_max - phi_min) / regression_points_amount;
    qDebug() << "delta Omega = " << d_omega;

    Eigen::VectorXd _omega_raw( regression_points_amount );
    Eigen::VectorXd omega = Eigen::Map<Matrix>(_omega_raw.data(), regression_points_amount, 1);
    omega = Eigen::Matrix<DType, Eigen::Dynamic, 1>::LinSpaced(
                regression_points_amount, phi_min, phi_max
            );
    //Matrix omega_std;
    //std::tie(omega_std, std::ignore, std::ignore) = standardize(omega);
    // scale
    Matrix omega_poly = generate_polynomial(omega, polynome_degree); // omega_std

    Eigen::VectorXd _fit_omega_raw( regression_points_amount );
    Eigen::VectorXd fit_omega = Eigen::Map<Matrix>(_fit_omega_raw.data(), regression_points_amount, 1);
    fit_omega = omega_poly * b;

    // Clamp fit
    for(uint i = 0; i < regression_points_amount; ++i) {
        if(fit_omega[i] < 0) fit_omega[i] = 0;
        // @ test
        //if(i < points_cnt)
        //qDebug() << i << "| " << f0[i] << "\t" << fit_omega[i];
    }

    // =================================
    // F2
    // =================================
    qDebug() << "Calculating F2...";
    //Eigen::VectorXd ones_n( regression_points_amount );
    //ones_n.fill(1);
    //double _sumF2 = (fit_omega * (ones_n - omega)).sum();

    double _sumF2 = 0;
    for(Eigen::Index i=0; i < regression_points_amount; ++i) {
        _sumF2 += fit_omega[i] * (1 - omega[i]);
    }
    qDebug() << "Sum F2 = " << _sumF2;

    double F2 = std::pow(F1, 2) * d_omega * (_sumF2 - 0.5 * (1 - sigma[0]));
    qDebug() << "F2 = " << F2;

    // =================================
    // F3
    // =================================
    qDebug() << "Calculating F3...";
    //double _sumF3 = (fit_omega * (ones_n - 2 * omega + omega * omega / 2)).sum();
    double _sumF3 = 0;
    for(Eigen::Index i=0; i < regression_points_amount; ++i) {
        _sumF3 += fit_omega[i] * (1 - 2*omega[i] + omega[i] * omega[i] / 2);
    }

    qDebug() << "Sum F3 = " << _sumF3;

    double F3 = std::pow(F1, 3) * d_omega * (_sumF3 - 0.5 * (1 - sigma[0]));
    qDebug() << "F3 = " << F3;

    // =================================
    // F4
    // =================================
    qDebug() << "Calculating F3...";
    double _sumF4 = 0;
    double _k1 = F3 / std::pow(F1, 3);
    double _k2 = F2 / std::pow(F1, 2);
    for(Eigen::Index i=0; i < regression_points_amount; ++i) {
        _sumF4 += fit_omega[i] * (_k1 - _k2*omega[i] + std::pow(omega[i], 2)/2 - std::pow(omega[i], 3)/6);
    }
    qDebug() << "Sum F4 = " << _sumF4;

    double F4 = std::pow(F1, 4) * d_omega * (_sumF4 - 0.5 * (1 - sigma[0]));
    qDebug() << "F4 = " << F4;

    // =================================
    // Return results
    // =================================
    double *results = new double[4];
    results[0] = F1;
    results[1] = F2;
    results[2] = F3;
    results[3] = F4;

    return results;
}

// Machine learning methods

std::tuple<SimouSolver::Matrix, double, SimouSolver::DType> SimouSolver::standardize(const SimouSolver::Matrix &v)
{
    assert(v.cols() == 1);
    auto m = v.colwise().mean();
    auto n = v.rows();
    DType sd = std::sqrt((v.rowwise() - m).array().pow(2).sum() / static_cast<DType>(n-1));

    Matrix sv = (v.rowwise() - m) / sd;
    return std::make_tuple(sv, m(0,0), sd);
}

SimouSolver::Matrix SimouSolver::generate_polynomial(const SimouSolver::Matrix &x, size_t degree)
{
    assert(x.cols() == 1);
    auto rows = x.rows(); // long long

    Matrix poly_x = Matrix::Zero(rows, static_cast<Eigen::Index>(degree)); // cast due sign
    // fill additional column for simpler vectorization
    {
        auto xv = poly_x.block(0,0,rows, 1);
        xv.setOnes();
    }
    // copy initial data
    {
        auto xv = poly_x.block(0,1,rows,1);
        xv = x;
    }
    // generate additionala terms
    for(size_t i = 2; i < degree; ++i) {
        auto xv = poly_x.block(0, i, rows, 1);
        xv = x.array().pow(static_cast<DType>(i));
    }
    return poly_x;
}

SimouSolver::Matrix SimouSolver::bgd(const SimouSolver::Matrix &x, const SimouSolver::Matrix &y, size_t batch_size, size_t n_epochs, DType lr)
{
//    defaults:
//    size_t batch_size = 8;
//    size_t n_epochs = 1000;
//    DType lr = 0.0015;

    auto rows = x.rows();
    auto cols = x.cols();

    size_t batches = rows / batch_size;  // some samples will be skipped
    Matrix b = Matrix::Zero(cols, 1);

    DType prev_cost = std::numeric_limits<DType>::max();
    for (size_t i = 0; i < n_epochs; ++i) {
    for (size_t bi = 0; bi < batches; ++bi) {
      auto s = bi * batch_size;
      auto batch_x = x.block(s, 0, batch_size, cols);
      auto batch_y = y.block(s, 0, batch_size, 1);

      auto yhat = batch_x * b;
      auto error = yhat - batch_y;

      auto grad =
          (batch_x.transpose() * error) / static_cast<DType>(batch_size);

      b = b - lr * grad;
    }

    DType cost = (y - x * b).array().pow(2.f).sum() / static_cast<DType>(rows);

    qDebug() << "BGD iteration : " << i << " Cost = " << cost;
    if (cost <= prev_cost)
      prev_cost = cost;
    else
      break;  // early stopping
    }
    return b;
}

SimouSolver::Matrix SimouSolver::regression(Eigen::VectorXd X, Eigen::VectorXd Y)
{
    const auto data_x = Eigen::Map<Matrix>(X.data(), X.rows(),1);
    Matrix x;
    std::tie(x, std::ignore, std::ignore) = standardize(data_x);

    const auto data_y = Eigen::Map<Matrix>(Y.data(), Y.rows(),1);
    Matrix y;
    DType ym{0}, ysd{0};
    std::tie(y, ym, ysd) = standardize(data_y);

    // scaling...  <not released yet>

    // solve normal equation
    Matrix poly_x = generate_polynomial(X, polynome_degree); // x
    Matrix b = (poly_x.transpose() * poly_x).ldlt().solve(poly_x.transpose() * Y); // y
    return b;
}
