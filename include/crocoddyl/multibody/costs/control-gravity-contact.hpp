///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2020-2021, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_CORE_COSTS_CONTROL_GRAVITY_CONTACT_HPP_
#define CROCODDYL_CORE_COSTS_CONTROL_GRAVITY_CONTACT_HPP_

#include "crocoddyl/core/cost-base.hpp"
#include "crocoddyl/multibody/states/multibody.hpp"
#include "crocoddyl/multibody/data/contacts.hpp"
#include "crocoddyl/core/utils/exception.hpp"

namespace crocoddyl {

/**
 * @brief Control gravity cost
 *
 * This cost function defines a residual vector as
 * \f$\mathbf{r}=\mathbf{u}-(g(q) - \sum J(q)^{\top} f_{\text{ext}})\f$,
 * where \f$\mathbf{u}\in~\mathbb{R}^{nu}\f$ is the current control input,
 * \f$J(q)\f$ the contact Jacobians, \f$f_{\text{ext}}\f$ the external forces
 * associated with the contacts, g the gravity torque corresponding to the
 * current configuration, \f$\mathbf{q}\in~\mathbb{R}^{nq}\f$ the current
 * position joints input.
 * Note that the dimension of the residual vector is obtained from `nu`.
 *
 * Both cost and residual derivatives are computed analytically.
 * For the computation of the cost Hessian, we use the Gauss-Newton
 * approximation, e.g. \f$\mathbf{l_{xx}} = \mathbf{l_{x}}^T \mathbf{l_{x}} \f$.
 *
 * As described in CostModelAbstractTpl(), the cost value and its derivatives
 * are calculated by `calc` and `calcDiff`, respectively.
 *
 * \sa `CostModelAbstractTpl`, `calc()`, `calcDiff()`, `createData()`
 */
template <typename _Scalar>
class CostModelControlGravContactTpl : public CostModelAbstractTpl<_Scalar> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef CostModelAbstractTpl<Scalar> Base;
  typedef CostDataControlGravContactTpl<Scalar> Data;
  typedef CostDataAbstractTpl<Scalar> CostDataAbstract;
  typedef StateMultibodyTpl<Scalar> StateMultibody;
  typedef ActivationModelAbstractTpl<Scalar> ActivationModelAbstract;
  typedef ActuationModelAbstractTpl<Scalar> ActuationModelAbstract;
  typedef ActivationModelQuadTpl<Scalar> ActivationModelQuad;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  /**
   * @brief Initialize the control gravity contact cost model
   *
   * @param[in] state       State of the multibody system
   * @param[in] activation  Activation model
   * @param[in] nu          Dimension of the control vector
   */
  CostModelControlGravContactTpl(boost::shared_ptr<StateMultibody> state,
                                 boost::shared_ptr<ActivationModelAbstract> activation, const std::size_t nu);

  /**
   * @brief Initialize the control gravity contact cost model
   *
   * The default `nu` value is obtained from `StateAbstractTpl::get_nv()`.
   *
   * @param[in] state       State of the multibody system
   * @param[in] activation  Activation model
   */
  CostModelControlGravContactTpl(boost::shared_ptr<StateMultibody> state,
                                 boost::shared_ptr<ActivationModelAbstract> activation);

  /**
   * @brief Initialize the control gravity contact cost model
   *
   * We use `ActivationModelQuadTpl` as a default activation model (i.e.
   * \f$a=\frac{1}{2}\|\mathbf{r}\|^2\f$).
   *
   * @param[in] state  State of the multibody system
   * @param[in] nu     Dimension of the control vector
   */
  CostModelControlGravContactTpl(boost::shared_ptr<StateMultibody> state, const std::size_t nu);

  /**
   * @brief Initialize the control gravity contact cost model
   *
   * The default `nu` value is obtained from `StateAbstractTpl::get_nv()`.
   * We use `ActivationModelQuadTpl` as a default activation model (i.e.
   * \f$a=\frac{1}{2}\|\mathbf{r}\|^2\f$).
   *
   * @param[in] state  State of the multibody system
   */
  explicit CostModelControlGravContactTpl(boost::shared_ptr<StateMultibody> state);
  virtual ~CostModelControlGravContactTpl();

  /**
   * @brief Compute the control gravity contact cost
   *
   * @param[in] data  Control cost data
   * @param[in] x     State point \f$\mathbf{x}\in\mathbb{R}^{ndx}\f$
   * @param[in] u     Control input \f$\mathbf{u}\in\mathbb{R}^{nu}\f$
   */
  virtual void calc(const boost::shared_ptr<CostDataAbstract> &data, const Eigen::Ref<const VectorXs> &x,
                    const Eigen::Ref<const VectorXs> &u);

  /**
   * @brief Compute the derivatives of the control gravity contact cost
   *
   * @param[in] data  Control cost data
   * @param[in] x     State point \f$\mathbf{x}\in\mathbb{R}^{ndx}\f$
   * @param[in] u     Control input \f$\mathbf{u}\in\mathbb{R}^{nu}\f$
   */
  virtual void calcDiff(const boost::shared_ptr<CostDataAbstract> &data, const Eigen::Ref<const VectorXs> &x,
                        const Eigen::Ref<const VectorXs> &u);

  virtual boost::shared_ptr<CostDataAbstract> createData(DataCollectorAbstract *const data);

 protected:
  using Base::activation_;
  using Base::nu_;
  using Base::state_;
  using Base::unone_;

 private:
  typename StateMultibody::PinocchioModel pin_model_;
};

template <typename _Scalar>
struct CostDataControlGravContactTpl : public CostDataAbstractTpl<_Scalar> {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef CostDataAbstractTpl<Scalar> Base;
  typedef StateMultibodyTpl<Scalar> StateMultibody;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef pinocchio::DataTpl<Scalar> PinocchioData;
  typedef typename MathBase::MatrixXs MatrixXs;

  template <template <typename Scalar> class Model>
  CostDataControlGravContactTpl(Model<Scalar> *const model, DataCollectorAbstract *const data)
      : Base(model, data),
        dg_dq(model->get_state()->get_nv(), model->get_state()->get_nv()),
        Arr_dgdq(model->get_state()->get_nv(), model->get_state()->get_nv()),
        Arr_dtaudu(model->get_state()->get_nv(), model->get_nu()) {
    dg_dq.setZero();
    Arr_dgdq.setZero();
    Arr_dtaudu.setZero();

    StateMultibody *sm = static_cast<StateMultibody *>(model->get_state().get());
    pinocchio = PinocchioData(*(sm->get_pinocchio().get()));

    // Check that proper shared data has been passed
    DataCollectorActMultibodyInContactTpl<Scalar> *d =
        dynamic_cast<DataCollectorActMultibodyInContactTpl<Scalar> *>(shared);
    if (d == NULL) {
      throw_pretty(
          "Invalid argument: the shared data should be derived from "
          "DataCollectorActMultibodyInContactTpl");
    }
    // Avoids data casting at runtime
    // pinocchio = d->pinocchio;
    fext = d->contacts->fext;
    actuation = d->actuation;
  }

  PinocchioData pinocchio;
  pinocchio::container::aligned_vector<pinocchio::ForceTpl<Scalar> > fext;
  boost::shared_ptr<ActuationDataAbstractTpl<Scalar> > actuation;
  MatrixXs dg_dq;
  MatrixXs Arr_dgdq;
  MatrixXs Arr_dtaudu;
  using Base::activation;
  using Base::cost;
  using Base::Lu;
  using Base::Luu;
  using Base::Lx;
  using Base::Lxu;
  using Base::Lxx;
  using Base::r;
  using Base::shared;
};

}  // namespace crocoddyl

/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
#include "crocoddyl/multibody/costs/control-gravity-contact.hxx"

#endif  // CROCODDYL_MULTIBODY_COSTS_CONTROL_GRAVITY_CONTACT_HPP_
