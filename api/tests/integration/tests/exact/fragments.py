import sys
sys.path.append('../../common')
from env_indigo import *

indigo = Indigo()

t = indigo.loadMoleculeFromFile(joinPath('molecules/t_795.mol'))
q = indigo.loadMoleculeFromFile(joinPath('molecules/q_42.mol'))

print(indigo.exactMatch(q, t, 'ALL') is not None)
print(indigo.exactMatch(q, t, 'ALL -FRA') is not None)

print(indigo.exactMatch(q, t, '') is not None)
print(indigo.exactMatch(q, t, '-FRA') is not None)
