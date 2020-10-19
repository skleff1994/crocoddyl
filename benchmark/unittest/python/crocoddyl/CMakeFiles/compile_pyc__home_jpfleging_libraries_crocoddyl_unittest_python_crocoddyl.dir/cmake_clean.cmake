file(REMOVE_RECURSE
  "../crocoddyl/action.pyc"
  "../crocoddyl/activation.pyc"
  "../crocoddyl/actuation.pyc"
  "../crocoddyl/box_ddp.pyc"
  "../crocoddyl/box_kkt.pyc"
  "../crocoddyl/callbacks.pyc"
  "../crocoddyl/contact.pyc"
  "../crocoddyl/cost.pyc"
  "../crocoddyl/ddp.pyc"
  "../crocoddyl/diagnostic.pyc"
  "../crocoddyl/differential_action.pyc"
  "../crocoddyl/fddp.pyc"
  "../crocoddyl/floating_contact.pyc"
  "../crocoddyl/impact.pyc"
  "../crocoddyl/__init__.pyc"
  "../crocoddyl/integrated_action.pyc"
  "../crocoddyl/kkt.pyc"
  "../crocoddyl/qpsolvers.pyc"
  "../crocoddyl/robots.pyc"
  "../crocoddyl/shooting.pyc"
  "../crocoddyl/solver.pyc"
  "../crocoddyl/state.pyc"
  "../crocoddyl/unicycle.pyc"
  "../crocoddyl/utils.pyc"
  "locomotion/__init__.pyc"
  "locomotion/centroidal_phi.pyc"
  "locomotion/contact_sequence_wrapper.pyc"
  "locomotion/problem_definition.pyc"
  "locomotion/spline_utils.pyc"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/compile_pyc__home_jpfleging_libraries_crocoddyl_unittest_python_crocoddyl.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
