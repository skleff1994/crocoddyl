///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_MULTIBODY_COSTS_COM_POSITION_HPP_
#define CROCODDYL_MULTIBODY_COSTS_COM_POSITION_HPP_

#include "crocoddyl/multibody/fwd.hpp"
#include "crocoddyl/multibody/cost-base.hpp"
#include "crocoddyl/multibody/data/multibody.hpp"
#include "crocoddyl/core/utils/exception.hpp"
#include "crocoddyl/core/utils/deprecate.hpp"

namespace crocoddyl {

template <typename _Scalar>
class CostModelCoMPositionTpl : public CostModelAbstractTpl<_Scalar> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef CostModelAbstractTpl<Scalar> Base;
  typedef CostDataCoMPositionTpl<Scalar> Data;
  typedef StateMultibodyTpl<Scalar> StateMultibody;
  typedef CostDataAbstractTpl<Scalar> CostDataAbstract;
  typedef ActivationModelAbstractTpl<Scalar> ActivationModelAbstract;
  typedef ActivationModelQuadTpl<Scalar> ActivationModelQuad;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef typename MathBase::Vector3s Vector3s;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  CostModelCoMPositionTpl(boost::shared_ptr<StateMultibody> state,
                          boost::shared_ptr<ActivationModelAbstract> activation, const Vector3s& cref,
                          const std::size_t& nu);
  CostModelCoMPositionTpl(boost::shared_ptr<StateMultibody> state,
                          boost::shared_ptr<ActivationModelAbstract> activation, const Vector3s& cref);
  CostModelCoMPositionTpl(boost::shared_ptr<StateMultibody> state, const Vector3s& cref, const std::size_t& nu);
  CostModelCoMPositionTpl(boost::shared_ptr<StateMultibody> state, const Vector3s& cref);
  virtual ~CostModelCoMPositionTpl();

  virtual void calc(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x,
                    const Eigen::Ref<const VectorXs>& u);
  virtual void calcDiff(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x,
                        const Eigen::Ref<const VectorXs>& u);
  virtual boost::shared_ptr<CostDataAbstract> createData(DataCollectorAbstract* const data);

  DEPRECATED("Use set_reference<MathBaseTpl<Scalar>::Vector3s>()", void set_cref(const Vector3s& cref_in));
  DEPRECATED("Use get_reference<MathBaseTpl<Scalar>::Vector3s>()", const Vector3s& get_cref() const);

 protected:
  virtual void set_referenceImpl(const std::type_info& ti, const void* pv);
  virtual void get_referenceImpl(const std::type_info& ti, void* pv) const;

  using Base::activation_;
  using Base::nu_;
  using Base::state_;
  using Base::unone_;

 private:
  Vector3s cref_;
};

template <typename _Scalar>
struct CostDataCoMPositionTpl : public CostDataAbstractTpl<_Scalar> {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef CostDataAbstractTpl<Scalar> Base;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;
  typedef typename MathBase::Matrix3xs Matrix3xs;
  typedef typename MathBase::Matrix6xs Matrix6xs;
  typedef typename MathBase::Matrix6s Matrix6s;
  typedef typename MathBase::Vector6s Vector6s;

  template <template <typename Scalar> class Model>
  CostDataCoMPositionTpl(Model<Scalar>* const model, DataCollectorAbstract* const data)
      : Base(model, data), Arr_Jcom(3, model->get_state()->get_nv()) {
    Arr_Jcom.setZero();
    // Check that proper shared data has been passed
    DataCollectorMultibodyTpl<Scalar>* d = dynamic_cast<DataCollectorMultibodyTpl<Scalar>*>(shared);
    if (d == NULL) {
      throw_pretty("Invalid argument: the shared data should be derived from DataCollectorMultibody");
    }

    // Avoids data casting at runtime
    pinocchio = d->pinocchio;
  }

  pinocchio::DataTpl<Scalar>* pinocchio;
  Matrix3xs Arr_Jcom;

  using Base::activation;
  using Base::cost;
  using Base::Lu;
  using Base::Luu;
  using Base::Lx;
  using Base::Lxu;
  using Base::Lxx;
  using Base::r;
  using Base::Ru;
  using Base::Rx;
  using Base::shared;
};

}  // namespace crocoddyl

/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
#include "crocoddyl/multibody/costs/com-position.hxx"

#endif  // CROCODDYL_MULTIBODY_COSTS_COM_POSITION_HPP_
