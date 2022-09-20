import argparse

import numpy as np
import scipy.linalg # This import is required to reliably trigger segfault
#import scipy.ndimage # This import also works - not limited to scipy.linalg

import segfault._segfault as segfault

parser = argparse.ArgumentParser()
parser.add_argument('-n', type=int, default=100)
parser.add_argument('--order', type=int, default=24)
args = parser.parse_args()

np.show_config()
print(f'n_mats: {args.n} order: {args.order}')
shape = (args.n, args.order, args.order)
dmats = np.random.rand(*shape) + 1j*np.random.rand(*shape)
evals = np.zeros((dmats.shape[0], dmats.shape[1]), dtype=np.float64)

segfault.diagonalise(evals, dmats, 1)

print('Done!')
