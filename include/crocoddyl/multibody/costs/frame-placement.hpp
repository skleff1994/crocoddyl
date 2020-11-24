///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_MULTIBODY_COSTS_FRAME_PLACEMENT_HPP_
#define CROCODDYL_MULTIBODY_COSTS_FRAME_PLACEMENT_HPP_

#include "crocoddyl/multibody/fwd.hpp"
#include "crocoddyl/multibody/cost-base.hpp"
#include "crocoddyl/multibody/data/multibody.hpp"
#include "crocoddyl/multibody/frames.hpp"
#include "crocoddyl/core/utils/exception.hpp"
#include "crocoddyl/core/utils/deprecate.hpp"

namespace crocoddyl {

template <typename _Scalar>
class CostModelFramePlacementTpl : public CostModelAbstractTpl<_Scalar> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef CostModelAbstractTpl<Scalar> Base;
  typedef CostDataFramePlacementTpl<Scalar> Data;
  typedef StateMultibodyTpl<Scalar> StateMultibody;
  typedef CostDataAbstractTpl<Scalar> CostDataAbstract;
  typedef ActivationModelAbstractTpl<Scalar> ActivationModelAbstract;
  typedef ActivationModelQuadTpl<Scalar> ActivationModelQuad;
  typedef FramePlacementTpl<Scalar> FramePlacement;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  CostModelFramePlacementTpl(boost::shared_ptr<StateMultibody> state,
                             boost::shared_ptr<ActivationModelAbstract> activation, const FramePlacement& Fref,
                             const std::size_t& nu);
  CostModelFramePlacementTpl(boost::shared_ptr<StateMultibody> state,
                             boost::shared_ptr<ActivationModelAbstract> activation, const FramePlacement& Fref);
  CostModelFramePlacementTpl(boost::shared_ptr<StateMultibody> state, const FramePlacement& Fref,
                             const std::size_t& nu);
  CostModelFramePlacementTpl(boost::shared_ptr<StateMultibody> state, const FramePlacement& Fref);
  virtual ~CostModelFramePlacementTpl();

  virtual void calc(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x,
                    const Eigen::Ref<const VectorXs>& u);
  virtual void calcDiff(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x,
                        const Eigen::Ref<const VectorXs>& u);
  virtual boost::shared_ptr<CostDataAbstract> createData(DataCollectorAbstract* const data);

  DEPRECATED("Use set_reference<FramePlacementTpl<Scalar> >()", void set_Mref(const FramePlacement& Mref_in));
  DEPRECATED("Use get_reference<FramePlacementTpl<Scalar> >()", const FramePlacement& get_Mref() const);

 protected:
  virtual void set_referenceImpl(const std::type_info& ti, const void* pv);
  virtual void get_referenceImpl(const std::type_info& ti, void* pv) const;

  using Base::activation_;
  using Base::nu_;
  using Base::state_;
  using Base::unone_;

 private:
  FramePlacement Mref_;
  pinocchio::SE3Tpl<Scalar> oMf_inv_;
};

template <typename _Scalar>
struct CostDataFramePlacementTpl : public CostDataAbstractTpl<_Scalar> {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef CostDataAbstractTpl<Scalar> Base;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;
  typedef typename MathBase::Matrix6xs Matrix6xs;
  typedef typename MathBase::Matrix6s Matrix6s;
  typedef typename MathBase::Vector6s Vector6s;

  template <template <typename Scalar> class Model>
  CostDataFramePlacementTpl(Model<Scalar>* const model, DataCollectorAbstract* const data)
      : Base(model, data),
        J(6, model->get_state()->get_nv()),
        rJf(6, 6),
        fJf(6, model->get_state()->get_nv()),
        Arr_J(6, model->get_state()->get_nv()) {
    r.setZero();
    J.setZero();
    rJf.setZero();
    fJf.setZero();
    Arr_J.setZero();
    // Check that proper shared data has been passed
    DataCollectorMultibodyTpl<Scalar>* d = dynamic_cast<DataCollectorMultibodyTpl<Scalar>*>(shared);
    if (d == NULL) {
      throw_pretty("Invalid argument: the shared data should be derived from DataCollectorMultibody");
    }

    // Avoids data casting at runtime
    pinocchio = d->pinocchio;
  }

  pinocchio::DataTpl<Scalar>* pinocchio;
  Vector6s r;
  pinocchio::SE3Tpl<Scalar> rMf;
  Matrix6xs J;
  Matrix6s rJf;
  Matrix6xs fJf;
  Matrix6xs Arr_J;

  using Base::activation;
  using Base::cost;
  using Base::Lu;
  using Base::Luu;
  using Base::Lx;
  using Base::Lxu;
  using Base::Lxx;
  using Base::shared;
  // using Base::r;
  using Base::Ru;
  using Base::Rx;
};

}  // namespace crocoddyl

/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
#include "crocoddyl/multibody/costs/frame-placement.hxx"

#endif  // CROCODDYL_MULTIBODY_COSTS_FRAME_PLACEMENT_HPP_
