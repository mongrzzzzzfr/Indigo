import os
import sys

sys.path.append(os.path.normpath(os.path.join(os.path.abspath(__file__), '..', '..', '..', "common")))
from env_indigo import *

indigo = Indigo()

for idx, m in enumerate(indigo.iterateSmilesFile(joinPathPy("molecules/b2000.smi", __file__))):
    fp = m.fingerprint("full")
