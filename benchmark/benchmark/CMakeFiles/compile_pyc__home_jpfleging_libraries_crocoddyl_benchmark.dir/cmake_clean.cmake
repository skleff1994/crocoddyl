file(REMOVE_RECURSE
  "./unicycle_optctrl.pyc"
  "./lqr_optctrl.pyc"
  "./arm_manipulation_optctrl.pyc"
  "./bipedal_walk_optctrl.pyc"
  "./quadrupedal_gaits_optctrl.pyc"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/compile_pyc__home_jpfleging_libraries_crocoddyl_benchmark.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
