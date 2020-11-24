///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_MULTIBODY_COST_BASE_HPP_
#define CROCODDYL_MULTIBODY_COST_BASE_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "crocoddyl/multibody/fwd.hpp"
#include "crocoddyl/multibody/states/multibody.hpp"
#include "crocoddyl/core/data-collector-base.hpp"
#include "crocoddyl/core/activation-base.hpp"
#include "crocoddyl/core/utils/to-string.hpp"
#include "crocoddyl/core/activations/quadratic.hpp"

namespace crocoddyl {

template <typename _Scalar>
class CostModelAbstractTpl {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef CostDataAbstractTpl<Scalar> CostDataAbstract;
  typedef StateMultibodyTpl<Scalar> StateMultibody;
  typedef ActivationModelAbstractTpl<Scalar> ActivationModelAbstract;
  typedef ActivationModelQuadTpl<Scalar> ActivationModelQuad;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  CostModelAbstractTpl(boost::shared_ptr<StateMultibody> state, boost::shared_ptr<ActivationModelAbstract> activation,
                       const std::size_t& nu);
  CostModelAbstractTpl(boost::shared_ptr<StateMultibody> state, boost::shared_ptr<ActivationModelAbstract> activation);
  CostModelAbstractTpl(boost::shared_ptr<StateMultibody> state, const std::size_t& nr, const std::size_t& nu);
  CostModelAbstractTpl(boost::shared_ptr<StateMultibody> state, const std::size_t& nr);
  ~CostModelAbstractTpl();

  virtual void calc(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x,
                    const Eigen::Ref<const VectorXs>& u) = 0;
  virtual void calcDiff(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x,
                        const Eigen::Ref<const VectorXs>& u) = 0;
  virtual boost::shared_ptr<CostDataAbstract> createData(DataCollectorAbstract* const data);

  void calc(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x);
  void calcDiff(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const VectorXs>& x);

  const boost::shared_ptr<StateMultibody>& get_state() const;
  const boost::shared_ptr<ActivationModelAbstract>& get_activation() const;
  const std::size_t& get_nu() const;

  template <class ReferenceType>
  void set_reference(ReferenceType ref);

  template <class ReferenceType>
  ReferenceType get_reference() const;

 protected:
  virtual void set_referenceImpl(const std::type_info&, const void*);
  virtual void get_referenceImpl(const std::type_info&, void*) const;

  boost::shared_ptr<StateMultibody> state_;
  boost::shared_ptr<ActivationModelAbstract> activation_;
  std::size_t nu_;
  VectorXs unone_;
};

template <typename _Scalar>
struct CostDataAbstractTpl {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef ActivationDataAbstractTpl<Scalar> ActivationDataAbstract;
  typedef DataCollectorAbstractTpl<Scalar> DataCollectorAbstract;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  template <template <typename Scalar> class Model>
  CostDataAbstractTpl(Model<Scalar>* const model, DataCollectorAbstract* const data)
      : shared(data),
        activation(model->get_activation()->createData()),
        cost(Scalar(0.)),
        Lx(model->get_state()->get_ndx()),
        Lu(model->get_nu()),
        Lxx(model->get_state()->get_ndx(), model->get_state()->get_ndx()),
        Lxu(model->get_state()->get_ndx(), model->get_nu()),
        Luu(model->get_nu(), model->get_nu()),
        r(model->get_activation()->get_nr()),
        Rx(model->get_activation()->get_nr(), model->get_state()->get_ndx()),
        Ru(model->get_activation()->get_nr(), model->get_nu()) {
    Lx.setZero();
    Lu.setZero();
    Lxx.setZero();
    Lxu.setZero();
    Luu.setZero();
    r.setZero();
    Rx.setZero();
    Ru.setZero();
  }
  virtual ~CostDataAbstractTpl() {}

  DataCollectorAbstract* shared;
  boost::shared_ptr<ActivationDataAbstract> activation;
  Scalar cost;
  VectorXs Lx;
  VectorXs Lu;
  MatrixXs Lxx;
  MatrixXs Lxu;
  MatrixXs Luu;
  VectorXs r;
  MatrixXs Rx;
  MatrixXs Ru;
};

}  // namespace crocoddyl

/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
#include "crocoddyl/multibody/cost-base.hxx"

#endif  // CROCODDYL_MULTIBODY_COST_BASE_HPP_
