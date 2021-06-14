///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2019-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "python/crocoddyl/multibody/multibody.hpp"
#include "python/crocoddyl/core/state-base.hpp"
#include "crocoddyl/multibody/states/statelpf.hpp"

namespace crocoddyl {
namespace python {

void exposeStateLPF() {
  bp::class_<StateLPF, bp::bases<StateAbstract> >(
      "StateLPF",
      "LPF state defined using Pinocchio.\n\n"
      "Pinocchio defines operators for integrating or differentiating the robot's\n"
      "configuration space. And here we assume that the state is defined by the\n"
      "robot's configuration, joint velocities and torques (y=[q,v,u]). Generally speaking,\n"
      "q lies on the manifold configuration manifold (M) and v in its tangent space\n"
      "(Ty M). Additionally the Pinocchio allows us to compute analytically the\n"
      "Jacobians for the differentiate and integrate operators. Note that this code\n"
      "can be reused in any robot that is described through its Pinocchio model.",
      bp::init<boost::shared_ptr<pinocchio::Model>, bp::optional<std::size_t> >(
          bp::args("self", "pinocchioModel", "nu"),
          "Initialize the multibody state given a Pinocchio model.\n\n"
          ":param pinocchioModel: pinocchio model (i.e. multibody model)\n"
          ":param nu: size of control vector.")[bp::with_custodian_and_ward<1, 2>()])
      .def("zero", &StateLPF::zero, bp::args("self"),
           "Return the neutral robot configuration with zero velocity.\n\n"
           ":return neutral robot configuration with zero velocity")
      .def("rand", &StateLPF::rand, bp::args("self"),
           "Return a random reference state.\n\n"
           ":return random reference state")
      .def("diff", &StateLPF::diff_dx, bp::args("self", "y0", "y1"),
           "Operator that differentiates the two robot states.\n\n"
           "It returns the value of y1 [-] y0 operation. This operator uses the Lie\n"
           "algebra since the robot's root could lie in the SE(3) manifold.\n"
           ":param y0: current state (dim state.ny()).\n"
           ":param y1: next state (dim state.ny()).\n"
           ":return y1 - y0 value (dim state.ny()).")
      .def("integrate", &StateLPF::integrate_x, bp::args("self", "y", "dy"),
           "Operator that integrates the current robot state.\n\n"
           "It returns the value of y [+] dy operation. This operator uses the Lie\n"
           "algebra since the robot's root could lie in the SE(3) manifold.\n"
           "Futhermore there is no timestep here (i.e. dy = v*dt), note this if you're\n"
           "integrating a velocity v during an interval dt.\n"
           ":param y: current state (dim state.ny()).\n"
           ":param dy: displacement of the state (dim state.ndy()).\n"
           ":return y + dy value (dim state.ny()).")
      .def("Jdiff", &StateLPF::Jdiff_Js,
           Jdiffs(bp::args("self", "y0", "y1", "firstsecond"),
                  "Compute the partial derivatives of the diff operator.\n\n"
                  "Both Jacobian matrices are represented throught an identity matrix, with the exception\n"
                  "that the robot's root is defined as free-flying joint (SE(3)). By default, this\n"
                  "function returns the derivatives of the first and second argument (i.e.\n"
                  "firstsecond='both'). However we ask for a specific partial derivative by setting\n"
                  "firstsecond='first' or firstsecond='second'.\n"
                  ":param y0: current state (dim state.ny()).\n"
                  ":param y1: next state (dim state.ny()).\n"
                  ":param firstsecond: desired partial derivative\n"
                  ":return the partial derivative(s) of the diff(y0, y1) function"))
      .def("Jintegrate", &StateLPF::Jintegrate_Js,
           Jintegrates(bp::args("self", "y", "dy", "firstsecond"),
                       "Compute the partial derivatives of arithmetic addition.\n\n"
                       "Both Jacobian matrices are represented throught an identity matrix. with the exception\n"
                       "that the robot's root is defined as free-flying joint (SE(3)). By default, this\n"
                       "function returns the derivatives of the first and second argument (i.e.\n"
                       "firstsecond='both'). However we ask for a specific partial derivative by setting\n"
                       "firstsecond='first' or firstsecond='second'.\n"
                       ":param x: current state (dim state.ny()).\n"
                       ":param dy: displacement of the state (dim state.ndy()).\n"
                       ":param firstsecond: desired partial derivative\n"
                       ":return the partial derivative(s) of the integrate(x, dy) function"))
      .add_property("pinocchio",
                    bp::make_function(&StateLPF::get_pinocchio, bp::return_value_policy<bp::return_by_value>()),
                    "pinocchio model");
     //  .add_property("ny", bp::make_getter(&StateLPF::ny_, bp::return_internal_reference<>()));
     //  .add_property("nw", bp::make_getter(&StateLPF::ny_, bp::return_internal_reference<>()));
}

}  // namespace python
}  // namespace crocoddyl
