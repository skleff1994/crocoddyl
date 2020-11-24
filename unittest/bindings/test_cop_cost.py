import crocoddyl
import pinocchio
import example_robot_data
import numpy as np

from test_utils import NUMDIFF_MODIFIER, assertNumDiff

# Create robot model, data, state and actuation
ROBOT_MODEL = example_robot_data.loadICub().model
ROBOT_DATA = ROBOT_MODEL.createData()
ROBOT_STATE = crocoddyl.StateMultibody(ROBOT_MODEL)
ACTUATION = crocoddyl.ActuationModelFloatingBase(ROBOT_STATE)

# Create activation to bound the CoP
activation = crocoddyl.ActivationModelQuadraticBarrier(
    crocoddyl.ActivationBounds(np.array([0] * 4), np.array([np.inf] * 4)))

# Contact CoP position cost unittest
CONTACTS = crocoddyl.ContactModelMultiple(ROBOT_STATE, ACTUATION.nu)
CONTACT_6D_RF = crocoddyl.ContactModel6D(
    ROBOT_STATE, crocoddyl.FramePlacement(ROBOT_MODEL.getFrameId('r_sole'), pinocchio.SE3.Random()), ACTUATION.nu,
    pinocchio.utils.rand(2))
CONTACT_6D_LF = crocoddyl.ContactModel6D(
    ROBOT_STATE, crocoddyl.FramePlacement(ROBOT_MODEL.getFrameId('l_sole'), pinocchio.SE3.Random()), ACTUATION.nu,
    pinocchio.utils.rand(2))
CONTACTS.addContact("r_sole_contact", CONTACT_6D_RF)
CONTACTS.addContact("l_sole_contact", CONTACT_6D_LF)
COSTS = crocoddyl.CostModelSum(ROBOT_STATE, ACTUATION.nu)
CoPCostRF = crocoddyl.CostModelContactCoPPosition(
    ROBOT_STATE, activation, crocoddyl.FrameCoPSupport(ROBOT_MODEL.getFrameId('r_sole'), np.array([0.01, 0.01])),
    ACTUATION.nu)
CoPCostLF = crocoddyl.CostModelContactCoPPosition(
    ROBOT_STATE, activation, crocoddyl.FrameCoPSupport(ROBOT_MODEL.getFrameId('l_sole'), np.array([0.01, 0.01])),
    ACTUATION.nu)
COSTS.addCost("r_sole_cop", CoPCostRF, 1)
COSTS.addCost("l_sole_cop", CoPCostLF, 1)
MODEL = crocoddyl.DifferentialActionModelContactFwdDynamics(ROBOT_STATE, ACTUATION, CONTACTS, COSTS, 0., True)
DATA = MODEL.createData()

MODEL_ND = crocoddyl.DifferentialActionModelNumDiff(MODEL)
MODEL_ND.disturbance *= 10
dnum = MODEL_ND.createData()

x = ROBOT_STATE.rand()
u = pinocchio.utils.rand(ACTUATION.nu)
MODEL.calc(DATA, x, u)
MODEL.calcDiff(DATA, x, u)
MODEL_ND.calc(dnum, x, u)
MODEL_ND.calcDiff(dnum, x, u)
assertNumDiff(DATA.Fx, dnum.Fx, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)
assertNumDiff(DATA.Fu, dnum.Fu, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)
assertNumDiff(DATA.Lx, dnum.Lx, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)
assertNumDiff(DATA.Lu, dnum.Lu, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)

# Impulse CoP position cost unittest
IMPULSES = crocoddyl.ImpulseModelMultiple(ROBOT_STATE)
IMPULSE_6D_RF = crocoddyl.ImpulseModel6D(ROBOT_STATE, ROBOT_MODEL.getFrameId('r_sole'))
IMPULSE_6D_LF = crocoddyl.ImpulseModel6D(ROBOT_STATE, ROBOT_MODEL.getFrameId('l_sole'))
IMPULSES.addImpulse("r_sole_impulse", IMPULSE_6D_RF)
IMPULSES.addImpulse("l_sole_impulse", IMPULSE_6D_LF)
COSTS = crocoddyl.CostModelSum(ROBOT_STATE, 0)
CoPCostRF = crocoddyl.CostModelImpulseCoPPosition(
    ROBOT_STATE, activation, crocoddyl.FrameCoPSupport(ROBOT_MODEL.getFrameId('r_sole'), np.array([0.01, 0.01])))
CoPCostLF = crocoddyl.CostModelImpulseCoPPosition(
    ROBOT_STATE, activation, crocoddyl.FrameCoPSupport(ROBOT_MODEL.getFrameId('l_sole'), np.array([0.01, 0.01])))
COSTS.addCost("r_sole_cop", CoPCostRF, 1)
COSTS.addCost("l_sole_cop", CoPCostLF, 1)
MODEL = crocoddyl.ActionModelImpulseFwdDynamics(ROBOT_STATE, IMPULSES, COSTS, 0., 0., True)
DATA = MODEL.createData()

MODEL_ND = crocoddyl.ActionModelNumDiff(MODEL)
MODEL_ND.disturbance *= 10
dnum = MODEL_ND.createData()

x = ROBOT_STATE.rand()
u = pinocchio.utils.rand(0)
MODEL.calc(DATA, x, u)
MODEL.calcDiff(DATA, x, u)
MODEL_ND.calc(dnum, x, u)
MODEL_ND.calcDiff(dnum, x, u)
assertNumDiff(DATA.Fx, dnum.Fx, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)
assertNumDiff(DATA.Fu, dnum.Fu, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)
assertNumDiff(DATA.Lx, dnum.Lx, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)
assertNumDiff(DATA.Lu, dnum.Lu, NUMDIFF_MODIFIER *
              MODEL_ND.disturbance)  # threshold was 2.7e-2, is now 2.11e-4 (see assertNumDiff.__doc__)