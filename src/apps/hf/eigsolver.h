#ifndef EIGSOLVER_H_
#define EIGSOLVER_H_
#include <mra/mra.h>
#include <world/world.h>
#include <vector>

namespace madness
{
//***************************************************************************
// TYPEDEFS
typedef SharedPtr< FunctionFunctorInterface<double,3> > functorT;
typedef Function<double,3> funcT;
typedef Vector<double,3> coordT;
//***************************************************************************

//***************************************************************************
class IEigSolverObserver
{
public:
  virtual void iterateOutput(const std::vector<funcT>& phis,
      const std::vector<double>& eigs, const funcT& rho, const int& iter) = 0;

  virtual ~IEigSolverObserver() {};
};
//***************************************************************************

//***************************************************************************
class EigSolverOp
{
public:
  //*************************************************************************
  // Constructor
  EigSolverOp(World& world, double coeff, double thresh)
    :  _world(world), _coeff(coeff), _thresh(thresh) {}
  //*************************************************************************

  //*************************************************************************
  // Destructor
  virtual ~EigSolverOp() {}
  //*************************************************************************
  
  //*************************************************************************
  // Is there an orbitally-dependent term?
  virtual bool is_od() = 0;
  //*************************************************************************

  //*************************************************************************
  // Is there a density-dependent term?
  virtual bool is_rd() = 0;
  //*************************************************************************

  //*************************************************************************
  // Orbital-dependent portion of operator
  virtual funcT op_o(const std::vector<funcT>& phis, const funcT& psi)
  {
    funcT func = FunctionFactory<double,3>(_world);
    return func;
  }
  //*************************************************************************

  //*************************************************************************
  // Density-dependent portion of operator
  virtual funcT op_r(const funcT& rho, const funcT& psi)
  {
    funcT func = FunctionFactory<double,3>(_world);
    return func;
  }
  //*************************************************************************

  //*************************************************************************
  double coeff() {return _coeff;}
  //*************************************************************************
  
protected:
  //*************************************************************************
  World& world() {return _world;}
  //*************************************************************************

  //*************************************************************************
  bool isPrintingNode()
  {
    //bool rval = (_world.rank() == 0) ? true : false;
    bool rval = true;
    return rval;
  }
  //*************************************************************************

  //*************************************************************************
  double thresh() {return _thresh;}
  //*************************************************************************
  
private:
  //*************************************************************************
  World& _world;
  //*************************************************************************

  //*************************************************************************
  double _coeff;
  //*************************************************************************

  //*************************************************************************
  double _thresh;
  //*************************************************************************
};
//***************************************************************************

//***************************************************************************
class EigSolver
{
public:
  //*************************************************************************
  // Constructor
  EigSolver(World& world, std::vector<funcT> phis, std::vector<double> eigs,
      std::vector<EigSolverOp*> ops, double thresh);
  //*************************************************************************

  //*************************************************************************
  // Destructor
  virtual ~EigSolver();
  //*************************************************************************

  //*************************************************************************
  void solve(int maxits);
  //*************************************************************************

  //*************************************************************************
  double get_eig(int indx)
  {
    return _eigs[indx];
  }
  //*************************************************************************

  //*************************************************************************
  funcT get_phi(int indx)
  {
    return _phis[indx];
  }
  //*************************************************************************

  //*************************************************************************
  const std::vector<funcT>& phis()
  {
    return _phis; 
  }
  //*************************************************************************

  //*************************************************************************
  const std::vector<double>& eigs()
  {
    return _eigs; 
  }
  //*************************************************************************

  //*************************************************************************
  void addObserver(IEigSolverObserver* obs)
  {
    _obs.push_back(obs);
  }
  //*************************************************************************

  //*************************************************************************
  double fock_matrix_element(const funcT& phii, const funcT& phij);
  //*************************************************************************

  //*************************************************************************
  static funcT compute_rho(std::vector<funcT> phis, const World& world);
  //*************************************************************************

private:
  //*************************************************************************
  // List of the functions
  std::vector<funcT> _phis;
  //*************************************************************************

  //*************************************************************************
  // List of the eigenvalues
  std::vector<double> _eigs;
  //*************************************************************************

  //*************************************************************************
  // List of the ops
  std::vector<EigSolverOp*> _ops;
  //*************************************************************************

  //*************************************************************************
  World& _world;
  //*************************************************************************

  //*************************************************************************
  double _thresh;
  //*************************************************************************

  //*************************************************************************
  // List of the ops
  std::vector<IEigSolverObserver*> _obs;
  //*************************************************************************

  //*************************************************************************
  funcT _rho;
  //*************************************************************************

};
//***************************************************************************

}

#endif /*EIGSOLVER_H_*/

