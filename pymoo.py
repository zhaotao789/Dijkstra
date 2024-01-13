import numpy as np
from pymoo.core.problem import Problem
from pymoo.core.problem import ElementwiseProblem
from pymoo.algorithms.moo.nsga2 import NSGA2
from pymoo.operators.crossover.sbx import SBX
from pymoo.operators.mutation.pm import PM
from pymoo.operators.sampling.rnd import FloatRandomSampling
from pymoo.termination import get_termination
from pymoo.optimize import minimize
import matplotlib.pyplot as plt
from pymoo.termination.default import DefaultMultiObjectiveTermination

# 定义问题   按元素
# class MyProblem(ElementwiseProblem):
#
#     def __init__(self):
#         super().__init__(n_var=2,
#                          n_obj=2,
#                          n_ieq_constr=2,
#                          xl=np.array([-2,-2]),
#                          xu=np.array([2,2]))   # 变量数量/目标/约束/上下界
#
#     def _evaluate(self, x, out, *args, **kwargs):
#         f1 = 100 * (x[0]**2 + x[1]**2)
#         f2 = (x[0]-1)**2 + x[1]**2
#
#         g1 = 2*(x[0]-0.1) * (x[0]-0.9) / 0.18
#         g2 = - 20*(x[0]-0.4) * (x[0]-0.6) / 4.8
#
#         out["F"] = [f1, f2]  # 目标
#         out["G"] = [g1, g2]  # 约束

# 定义问题   按数组
class MyProblem(Problem):
    def __init__(self):
        super().__init__(n_var=2,
                         n_obj=2,
                         n_ieq_constr=2,
                         xl=-2.0,
                         xu=2.0)

    def _evaluate(self, x, out, *args, **kwargs):
        f1 = 100 * (x[:, 0]**2 + x[:, 1]**2)
        f2 = (x[:, 0]-1)**2 + x[:, 1]**2

        g1 = 2 * (x[:, 0] - 0.1) * (x[:, 0] - 0.9) / 0.18
        g2 = - 20*(x[:, 0]-0.4) * (x[:, 0]-0.6) / 4.8

        out["F"] = [f1, f2]  # 目标
        out["G"] = [g1, g2]  # 约束

problem = MyProblem()

algorithm = NSGA2(
    pop_size=40,  # 种群规模
    n_offsprings=10,  # 每代数量
    sampling=FloatRandomSampling(),
    crossover=SBX(prob=0.9, eta=15),
    mutation=PM(eta=20),
    eliminate_duplicates=True  # 重复检查，剔除相似个体
)

# 设置终止条件
# termination = get_termination("n_gen", 40)   # 迭代次数
termination = DefaultMultiObjectiveTermination(
    xtol=1e-8,
    cvtol=1e-6,
    ftol=0.0025,
    period=30,
    n_max_gen=1000,
    n_max_evals=100000
)


res = minimize(problem,
               algorithm,
               termination,
               seed=1,
               save_history=True,
               verbose=True)

X = res.X
F = res.F

# 查看解空间
xl, xu = problem.bounds()
plt.figure(figsize=(7, 5))
plt.scatter(X[:, 0], X[:, 1], s=30, facecolors='none', edgecolors='r')
plt.xlim(xl[0], xu[0])
plt.ylim(xl[1], xu[1])
plt.title("Design Space")
plt.show()

# 查看目标值
plt.figure(figsize=(7, 5))
plt.scatter(F[:, 0], F[:, 1], s=30, facecolors='none', edgecolors='blue')
plt.title("Objective Space")
plt.show()