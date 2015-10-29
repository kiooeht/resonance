#pragma once
#ifndef SOLVER_H
#define SOLVER_H

#include <tuple>
#include "typedefs.h"
#include "system.h"

struct SolverResults
{
    MatrixXc H;
    MatrixXr O;
    std::vector<complex>  eigenvalues;
    std::vector<VectorXc> eigenvectors;
};

class Solver
{
public:
    Solver(System*);
    virtual ~Solver();

    //Compute and solve for ALL the Hamiltonian elements.
    virtual SolverResults solve(const Basis&, real theta)=0;

    //Compute Hamiltonian elements only for one row/col
    virtual SolverResults solveRow(const Basis&, real theta, SolverResults& cache, uint row)=0;

    virtual real overlap(const CGaussian&, const CGaussian&)=0;

    const SolverResults& results() { return sresults; }

protected:
    System*  system;
    SolverResults sresults;
};

class CpuSolver : public Solver
{
public:
    CpuSolver(System*);
    ~CpuSolver();

    SolverResults solve(const Basis&, real theta);
    SolverResults solveRow(const Basis&, real theta, SolverResults& cache, uint row);

    real overlap(const CGaussian&, const CGaussian&);

private:
    complex kinetic  (const CGaussian&, const CGaussian&, real,real);
    complex gaussianV(real v0, real r0, real theta, real over, real c_ij);

    real genc_ij(const CGaussian& A, const CGaussian& B, uint i, uint j);

    SolverResults compute(MatrixXc& H, MatrixXr& O);
    SolverResults computeHermition(MatrixXc& H, MatrixXr& O);

    std::tuple<Basis,std::vector<int>,uint> symmetrize(const CGaussian&);
};

#endif
