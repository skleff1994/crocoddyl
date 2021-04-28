///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_CORE_INTEGRATOR_LPF_HPP_
#define CROCODDYL_CORE_INTEGRATOR_LPF_HPP_

#include "crocoddyl/core/fwd.hpp"
#include "crocoddyl/core/action-base.hpp"
#include "crocoddyl/core/diff-action-base.hpp"

namespace crocoddyl {

template <typename _Scalar>
class IntegratedActionModelLPFTpl : public ActionModelAbstractTpl<_Scalar> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef ActionModelAbstractTpl<Scalar> Base;
  typedef IntegratedActionDataLPFTpl<Scalar> Data;
  typedef ActionDataAbstractTpl<Scalar> ActionDataAbstract;
  typedef DifferentialActionModelAbstractTpl<Scalar> DifferentialActionModelAbstract;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  IntegratedActionModelLPFTpl(boost::shared_ptr<DifferentialActionModelAbstract> model, const Scalar& nu = 0,
                                const Scalar& time_step = Scalar(1e-3), const bool& with_cost_residual = true, const Scalar& alpha = 0);
  virtual ~IntegratedActionModelLPFTpl();

  virtual void calc(const boost::shared_ptr<ActionDataAbstract>& data, const Eigen::Ref<const VectorXs>& y,
                    const Eigen::Ref<const VectorXs>& w);
  virtual void calcDiff(const boost::shared_ptr<ActionDataAbstract>& data, const Eigen::Ref<const VectorXs>& y,
                        const Eigen::Ref<const VectorXs>& w);
  virtual void setAlpha(const Scalar& f_c);
  virtual boost::shared_ptr<ActionDataAbstract> createData();
  virtual bool checkData(const boost::shared_ptr<ActionDataAbstract>& data);

  virtual void quasiStatic(const boost::shared_ptr<ActionDataAbstract>& data, Eigen::Ref<VectorXs> u,
                           const Eigen::Ref<const VectorXs>& x, const std::size_t& maxiter = 100,
                           const Scalar& tol = Scalar(1e-9));

  const boost::shared_ptr<DifferentialActionModelAbstract>& get_differential() const;
  const Scalar& get_dt() const;
  const Scalar& get_alpha() const;

  void set_dt(const Scalar& dt);
  void set_alpha(const Scalar& alpha);
  void set_differential(boost::shared_ptr<DifferentialActionModelAbstract> model);

 protected:
  using Base::has_control_limits_;  //!< Indicates whether any of the control limits are active
  using Base::nr_;                  //!< Dimension of the cost residual
  using Base::nu_;                  //!< Control dimension
  using Base::state_;               //!< Model of the state
  using Base::u_lb_;                //!< Lower control limits
  using Base::u_ub_;                //!< Upper control limits
  using Base::unone_;               //!< Neutral state
  std::size_t nw_;                          //!< Unfiltered torque dimension
  std::size_t ny_;                          //!< Augmented state dimension
  // StateLPF state2_;

 private:
  boost::shared_ptr<DifferentialActionModelAbstract> differential_;
  Scalar time_step_;
  Scalar time_step2_;
  Scalar alpha_;
  bool with_cost_residual_;
  bool enable_integration_;
};

template <typename _Scalar>
struct IntegratedActionDataLPFTpl : public ActionDataAbstractTpl<_Scalar> {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef ActionDataAbstractTpl<Scalar> Base;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  template <template <typename Scalar> class Model>
  explicit IntegratedActionDataLPFTpl(Model<Scalar>* const model) : Base(model) {
        differential = model->get_differential()->createData();
        const std::size_t& ndy = model->get_state()->get_ndx() + model->get_nu(); // replace by ndy from stateLPF ?
        dx = VectorXs::Zero(ndy);
  }
  virtual ~IntegratedActionDataLPFTpl() {}

  boost::shared_ptr<DifferentialActionDataAbstractTpl<Scalar> > differential;
  VectorXs dx;

  using Base::cost;
  using Base::Fu;
  using Base::Fx;
  using Base::Lu;
  using Base::Luu;
  using Base::Lx;
  using Base::Lxu;
  using Base::Lxx;
  using Base::r;
  using Base::xnext;
};

}  // namespace crocoddyl

/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
#include "crocoddyl/core/integrator/lpf.hxx"

#endif  // CROCODDYL_CORE_INTEGRATOR_LPF_HPP_

