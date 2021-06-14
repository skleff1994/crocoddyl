///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "crocoddyl/core/utils/exception.hpp"
#include "crocoddyl/core/integrator/lpf.hpp"

namespace crocoddyl {

template <typename Scalar>
IntegratedActionModelLPFTpl<Scalar>::IntegratedActionModelLPFTpl(
    boost::shared_ptr<DifferentialActionModelAbstract> model, const Scalar& nu, const Scalar& time_step, const bool& with_cost_residual, const Scalar& alpha)
    : Base(model->get_state(), model->get_nu(), model->get_nr()),
      differential_(model),
      time_step_(time_step),
      time_step2_(time_step * time_step),
      with_cost_residual_(with_cost_residual),
      alpha_(alpha),
      nw_(model->get_nu()),
      ny_(model->get_state()->get_nx() + model->get_nu()),
      enable_integration_(true) {
  Base::set_u_lb(differential_->get_u_lb());
  Base::set_u_ub(differential_->get_u_ub());
  if (time_step_ < Scalar(0.)) {
    time_step_ = Scalar(1e-3);
    time_step2_ = time_step_ * time_step_;
    std::cerr << "Warning: dt should be positive, set to 1e-3" << std::endl;
  }
  if (time_step == Scalar(0.)) {
    enable_integration_ = false;
  }
  // overwrite the state
  // boost::shared_ptr<StateMultibody> state = boost::static_pointer_cast<StateMultibody>(state_);
  boost::shared_ptr<DifferentialActionModelFreeFwdDynamics> dam = boost::static_pointer_cast<DifferentialActionModelFreeFwdDynamics>(differential_);
  // state_ = StateLPFTpl<Scalar> (boost::static_pointer_cast<DifferentialActionModelFreeFwdDynamics>(differential_)->get_pinocchio(), nu_);
  // state_ = StateLPFTpl<Scalar> (dam->get_pinocchio(), nu_);
  // state_ = StateLPFTpl<Scalar> (boost::static_pointer_cast<DifferentialActionModelFreeFwdDynamicsTpl<Scalar>>(differential_)->get_pinocchio(), nu_);
}

// template <typename Scalar>
// IntegratedActionModelLPFTpl<Scalar>::IntegratedActionModelLPFTpl(
//     boost::shared_ptr<DifferentialActionModelAbstract> model, const Scalar& nu, const Scalar& time_step, const bool& with_cost_residual, const Scalar& alpha)
//     : Base(boost::make_shared<StateAbstract>(model->get_state()->get_nx()+model->get_nu()), model->get_state()->get_nx()+model->get_nu()), model->get_nu(), model->get_nr()),
//       differential_(model),
//       time_step_(time_step),
//       time_step2_(time_step * time_step),
//       with_cost_residual_(with_cost_residual),
//       alpha_(alpha),
//       nw_(model->get_nu()),
//       ny_(model->get_state()->get_nx() + model->get_nu()),
//       enable_integration_(true) {
//   Base::set_u_lb(differential_->get_u_lb());
//   Base::set_u_ub(differential_->get_u_ub());
//   if (time_step_ < Scalar(0.)) {
//     time_step_ = Scalar(1e-3);
//     time_step2_ = time_step_ * time_step_;
//     std::cerr << "Warning: dt should be positive, set to 1e-3" << std::endl;
//   }
//   if (time_step == Scalar(0.)) {
//     enable_integration_ = false;
//   }
//   // overwrite the state
//   // state2_ = StateLPFTpl<Scalar> (static_cast<DifferentialActionModelFreeFwdDynamicsTpl<Scalar>>(differential_)->get_pinocchio(), nu_);
// }

template <typename Scalar>
IntegratedActionModelLPFTpl<Scalar>::~IntegratedActionModelLPFTpl() {}

template <typename Scalar>
void IntegratedActionModelLPFTpl<Scalar>::calc(const boost::shared_ptr<ActionDataAbstract>& data,
                                               const Eigen::Ref<const VectorXs>& y,
                                               const Eigen::Ref<const VectorXs>& w) {

  const Eigen::Ref<const VectorXs>& x = y.head(state_->get_nx());          // get q,v_q
  const Eigen::Ref<const VectorXs>& u = alpha_*y.tail(nw_) + (1-alpha_)*w; // get tau_q+

  if (static_cast<std::size_t>(x.size()) != state_->get_nx()) {
    throw_pretty("Invalid argument: "
                 << "x has wrong dimension (it should be " + std::to_string(state_->get_nx()) + ")" +
                 "but it is " + std::to_string(x.size()));
  }
  if (static_cast<std::size_t>(u.size()) != nu_) {
    throw_pretty("Invalid argument: "
                 << "u has wrong dimension (it should be " + std::to_string(nu_) + ")");
  }

  const std::size_t& nv = differential_->get_state()->get_nv();

  // Static casting the data
  boost::shared_ptr<Data> d = boost::static_pointer_cast<Data>(data);

  // Computing the acceleration and cost
  differential_->calc(d->differential, x, u); // get a_q = DAM(q, v_q, tau_q+) // here use tau or tau+

  // Computing the next state (discrete time)
  const Eigen::VectorBlock<const Eigen::Ref<const VectorXs>, Eigen::Dynamic> v =
      x.tail(differential_->get_state()->get_nv());
  const VectorXs& a = d->differential->xout;
  if (enable_integration_) {
    d->dx.head(nv).noalias() = v * time_step_ + a * time_step2_;
    d->dx.tail(nv).noalias() = a * time_step_;
    d->cost = time_step_ * d->differential->cost;
    // const VectorXs& xplus = VectorXs::Zero(differential_->get_state()->get_nx());
    differential_->get_state()->integrate(x, d->dx, d->xnext); // get (q+, vq+) = int(a_q)
    // // NOT SURE HOW TO SET THIS PART FROM DIFFERENTIAL
    // d->xnext.head(differential_->get_state()->get_nx()).noalias() = xplus;
    // d->xnext.tail(differential_->get_state()->get_nx()).noalias() = u;
  } else {
    d->dx.setZero();
    d->xnext = y;
    d->cost = d->differential->cost;
  }

  // Updating the cost value
  if (with_cost_residual_) {
    d->r = d->differential->r;
  }
}

template <typename Scalar>
void IntegratedActionModelLPFTpl<Scalar>::setAlpha(const Scalar& f_c) {
  // Changes the parameter alpha according to the cut-off frequency
  if (f_c > 0 && time_step_ != 0){
    double omega = 1 / (2 * 3.14 * time_step_ * f_c);
    alpha_ = omega/(omega + 1);
  }
  else {
    alpha_ = 0;
  }
}

template <typename Scalar>
void IntegratedActionModelLPFTpl<Scalar>::calcDiff(const boost::shared_ptr<ActionDataAbstract>& data,
                                                     const Eigen::Ref<const VectorXs>& x,
                                                     const Eigen::Ref<const VectorXs>& u) {
  
//   const Eigen::Ref<const VectorXs>& x = y.head(state_->get_nx());
//   const Eigen::Ref<const VectorXs>& u = y.tail(state_->get_nu());

  if (static_cast<std::size_t>(x.size()) != state_->get_nx()) {
    throw_pretty("Invalid argument: "
                 << "x has wrong dimension (it should be " + std::to_string(state_->get_nx()) + ")");
  }
  if (static_cast<std::size_t>(u.size()) != nu_) {
    throw_pretty("Invalid argument: "
                 << "u has wrong dimension (it should be " + std::to_string(nu_) + ")");
  }

  const std::size_t& nv = differential_->get_state()->get_nv();

  // Static casting the data
  boost::shared_ptr<Data> d = boost::static_pointer_cast<Data>(data);

  // Computing the derivatives for the time-continuous model (i.e. differential model)
  differential_->calcDiff(d->differential, x, u);

  if (enable_integration_) {
    const MatrixXs& da_dx = d->differential->Fx;
    const MatrixXs& da_du = d->differential->Fu;
    d->Fx.topRows(nv).noalias() = da_dx * time_step2_;
    d->Fx.bottomRows(nv).noalias() = da_dx * time_step_;
    d->Fx.topRightCorner(nv, nv).diagonal().array() += Scalar(time_step_);

    d->Fu.topRows(nv).noalias() = da_du * time_step2_;
    d->Fu.bottomRows(nv).noalias() = da_du * time_step_;

    differential_->get_state()->JintegrateTransport(x, d->dx, d->Fx, second);
    differential_->get_state()->Jintegrate(x, d->dx, d->Fx, d->Fx, first, addto);
    differential_->get_state()->JintegrateTransport(x, d->dx, d->Fu, second);

    d->Lx.noalias() = time_step_ * d->differential->Lx;
    d->Lu.noalias() = time_step_ * d->differential->Lu;
    d->Lxx.noalias() = time_step_ * d->differential->Lxx;
    d->Lxu.noalias() = time_step_ * d->differential->Lxu;
    d->Luu.noalias() = time_step_ * d->differential->Luu;
  } else {
    differential_->get_state()->Jintegrate(x, d->dx, d->Fx, d->Fx);
    d->Fu.setZero();
    d->Lx = d->differential->Lx;
    d->Lu = d->differential->Lu;
    d->Lxx = d->differential->Lxx;
    d->Lxu = d->differential->Lxu;
    d->Luu = d->differential->Luu; 
  }
}

template <typename Scalar>
boost::shared_ptr<ActionDataAbstractTpl<Scalar> > IntegratedActionModelLPFTpl<Scalar>::createData() {
  return boost::allocate_shared<Data>(Eigen::aligned_allocator<Data>(), this);
}

template <typename Scalar>
bool IntegratedActionModelLPFTpl<Scalar>::checkData(const boost::shared_ptr<ActionDataAbstract>& data) {
  boost::shared_ptr<Data> d = boost::dynamic_pointer_cast<Data>(data);
  if (data != NULL) {
    return differential_->checkData(d->differential);
  } else {
    return false;
  }
}

template <typename Scalar>
const boost::shared_ptr<DifferentialActionModelAbstractTpl<Scalar> >&
IntegratedActionModelLPFTpl<Scalar>::get_differential() const {
  return differential_;
}

template <typename Scalar>
const Scalar& IntegratedActionModelLPFTpl<Scalar>::get_dt() const {
  return time_step_;
}

template <typename Scalar>
void IntegratedActionModelLPFTpl<Scalar>::set_dt(const Scalar& dt) {
  if (dt < 0.) {
    throw_pretty("Invalid argument: "
                 << "dt has positive value");
  }
  time_step_ = dt;
  time_step2_ = dt * dt;
}

template <typename Scalar>
const Scalar& IntegratedActionModelLPFTpl<Scalar>::get_alpha() const {
  return alpha_;
}

template <typename Scalar>
void IntegratedActionModelLPFTpl<Scalar>::set_alpha(const Scalar& alpha) {
  if (alpha < 0. || alpha > 1.) {
    throw_pretty("Invalid argument: "
                 << "alpha must be in [0, 1]");
  }
  alpha_ = alpha;
}

template <typename Scalar>
void IntegratedActionModelLPFTpl<Scalar>::set_differential(
    boost::shared_ptr<DifferentialActionModelAbstract> model) {
  const std::size_t& nu = model->get_nu();
  if (nu_ != nu) {
    nu_ = nu;
    unone_ = VectorXs::Zero(nu_);
  }
  nr_ = model->get_nr();
  state_ = model->get_state();
  differential_ = model;
  Base::set_u_lb(differential_->get_u_lb());
  Base::set_u_ub(differential_->get_u_ub());
}

template <typename Scalar>
void IntegratedActionModelLPFTpl<Scalar>::quasiStatic(const boost::shared_ptr<ActionDataAbstract>& data,
                                                        Eigen::Ref<VectorXs> u, const Eigen::Ref<const VectorXs>& x,
                                                        const std::size_t& maxiter, const Scalar& tol) {
  if (static_cast<std::size_t>(u.size()) != nu_) {
    throw_pretty("Invalid argument: "
                 << "u has wrong dimension (it should be " + std::to_string(nu_) + ")");
  }
  if (static_cast<std::size_t>(x.size()) != state_->get_nx()) {
    throw_pretty("Invalid argument: "
                 << "x has wrong dimension (it should be " + std::to_string(state_->get_nx()) + ")");
  }

  // Static casting the data
  boost::shared_ptr<Data> d = boost::static_pointer_cast<Data>(data);

  differential_->quasiStatic(d->differential, u, x, maxiter, tol);
}

}  // namespace crocoddyl