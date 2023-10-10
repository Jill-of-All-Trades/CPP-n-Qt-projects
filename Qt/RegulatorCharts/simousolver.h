#ifndef SIMOUSOLVER_H
#define SIMOUSOLVER_H

#include <math.h>
#include <QList>
#include <Eigen/Core>

class SimouSolver
{
public:
typedef double DType;
using Matrix = Eigen::Matrix<DType, Eigen::Dynamic, Eigen::Dynamic>;

private:
    QList<QPointF> *points = nullptr;

    double discrete_freq;
    size_t polynome_degree;
    uint regression_points_amount;

    // ML
    std::tuple<Matrix, double, DType> standardize(const Matrix& v);
    Matrix generate_polynomial(const Matrix& x, size_t degree);
    Matrix bgd(const Matrix& x, const Matrix& y, size_t batch_size = 8, size_t n_epochs = 1000, DType lr = 0.0015);

    Matrix regression(Eigen::VectorXd X, Eigen::VectorXd y);

public:
    SimouSolver(QList<QPointF> *points, size_t polynome_degree = 3);

    void CalcDiscreteFrequency();
    double GetDiscreteFrequency() const;

    void SetPolynomialDegree(size_t degree);
    size_t GetPolynomailDegree() const;

    void SetRegressionPointsAmount(uint amount);
    uint GetRegressionPointsAmount() const;

    double* Solve();

};

#endif // SIMOUSOLVER_H
