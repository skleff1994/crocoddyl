file(REMOVE_RECURSE
  "./__init__.pyc"
  "./deprecated.pyc"
  "./deprecation.pyc"
  "utils/__init__.pyc"
  "utils/pendulum.pyc"
  "utils/biped.pyc"
  "utils/quadruped.pyc"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/compile_pyc__home_jpfleging_libraries_crocoddyl_bindings_python_crocoddyl.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
