import os
import subprocess
import argparse
from argparse import Namespace
import itertools
from copy import deepcopy

import numpy as np
import pandas as pd 

try:
	import matplotlib
	import matplotlib.pyplot as plt
	from matplotlib.transforms import Affine2D
	matplotlib_loaded = True
except Exception:
	matplotlib_loaded = False




#example_file_list = ['Aesthetik', 'Akkordeon', 'Analog-digital-Wandler',
#                     'Betriebssportgemeinschaft', 'Bratsche', 'Buch',
#                     'Dateienunterverzeichnis','Schwarzweiss-Malerei']

example_file_list = ['Dateienunterverzeichnis','Schwarzweiss-Malerei']

#example_bounds_list = [4,5,9,7,3,2,9,6

example_bounds_list = [9,6]

test_list = ['Analog-digital-Wandler']

inputPath  = './Examples/'
outputPath = './Benchmark/'
logPath    = './LOG/'




#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#
class TO_Benchmarks():
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	"""A class for TO_Benchamrks"""
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def __init__( self ):
		#self.args = args
		if ( not os.path.exists( logPath ) ):
			os.mkdir( logPath )
		#if ( not self.args.files ):
		#		self.args.files = example_list
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def eval_hyper_hyper( self ):
		maxIterations_list = [1000]
		maxCostEvaluations_list = [1e5 * x for x in range(1,11)]
		minRhoEnd_list = [1e-3,1e-4,1e-5,1e-6]
		args = Namespace( maxIterations      = ['--maxIterations', maxIterations_list],
			              maxCostEvaluations = ['--maxCostEvaluations', maxCostEvaluations_list],
			              minRhoEnd          = ['--rhoEnd', minRhoEnd_list],
			            )
		self.run( files = example_file_list, bounds = example_bounds_list, args= args )
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def eval_test( self ):
		#maxIterations_list = [2]
		#minRhoEnd_list =[1e-3, 1e-4]
		#args = Namespace( maxIterations = ['--maxIterations', maxIterations_list], minRhoEnd = ['--rhoEnd', minRhoEnd_list], )
		self.run( files = ['Analog-digital-Wandler'], bounds = [9], args = None )
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def eval_hyper_hyper_test( self ):
		maxIterations_list = [1]
		maxCostEvaluations_list = [1e5 * x for x in range(1,11)]
		minRhoEnd_list = [1e-3,1e-4,1e-5,1e-6]
		args = Namespace( maxIterations      = ['--maxIterations', maxIterations_list],
			              maxCostEvaluations = ['--maxCostEvaluations', maxCostEvaluations_list],
			              minRhoEnd          = ['--rhoEnd', minRhoEnd_list],
			            )
		self.run( files = example_file_list, bounds = example_bounds_list, args= args )
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#

#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def pd_str_to_np( self, df_entry ):
		return np.array( df_entry.split(' ') ).astype(np.float)
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def plot_demo( self, file = 'LOG_Example.csv'):
		assert matplotlib_loaded, 'ERROR in "plot_demo()": matplotlib could not be loaded. Is it installed?'
		assert os.path.exists( file ), 'ERROR in "plot_demo()": the specified file {:} does not exist.'
		df = self.read_LOG( file )
		fig, axes = plt.subplots(2,2)

		axes[0,0].plot( self.pd_str_to_np( df['res_ftmp_array'][0] ) )

		axes[0,1].plot( self.pd_str_to_np( df['res_f0_time_array'][0] ), self.pd_str_to_np( df['res_f0_value_array'][0] ), color = 'navy' )
		axes[0,1].scatter( self.pd_str_to_np( df['ini_f0_time_array'][0] ), self.pd_str_to_np( df['ini_f0_value_array'][0] ), color = 'orange')
		for boundary in self.pd_str_to_np( df['res_boundary_array'][0] ):
			axes[0,1].axvspan( boundary, boundary, ymax=0.9, color = 'lightgray', ls='--', alpha = 0.5)

		for boundary in self.pd_str_to_np( df['ini_boundary_array'][0] ):
			axes[0,1].axvspan( boundary, boundary, ymin = 0.9, color = 'black', alpha = 0.5)

		slopes     = self.pd_str_to_np( df['res_t_slope_array'][0] )
		offsets    = self.pd_str_to_np( df['res_t_offset_array'][0] )
		boundaries = self.pd_str_to_np( df['res_boundary_array'][0] )
		assert len(slopes) == len(offsets) == len(boundaries)-1, 'FATAl ERROR, the target slopes and offsets and/or boundary sizes do nnot match!'
		for index, slope in enumerate( slopes ):
			print(offsets[index])
			x = [ boundaries[index], boundaries[index+1] ]
			y = [ slopes[index]* (x[0]-boundaries[index]) + offsets[index], slopes[index]* (x[1]-boundaries[index]) + offsets[index] ]
			axes[0,1].plot( x, y, color ='lightgray')



		ax_labels = [ [['Iteration', 'ftmp'], ['Time [s]', 'F0 [Hz]']], [['Iteration', 'fmin'], ['Iteration', 'fmin']] ]
		for i, axs in enumerate( axes ):
			for j, ax in enumerate( axs ):
				ax.set( xlabel = ax_labels[i][j][0], ylabel = ax_labels[i][j][1] )
		ftmp_min = 1e6
		fmin = []
		for ftmp in self.pd_str_to_np( df['res_ftmp_array'][0] ):
			if ftmp < ftmp_min:
				ftmp_min = ftmp
				fmin.append(ftmp_min)
			else:
				fmin.append( min(fmin) )
		axes[1,0].plot( fmin )

		axes[1,1].get_xaxis().set_visible(False)
		axes[1,1].get_yaxis().set_visible(False)
		axes[1,1].text(0.05, 0.9, 'file: {}'.format(df['utterance'][0]))
		axes[1,1].text(0.05, 0.8, 'SS:')
		axes[1,1].text(0.05, 0.7, r'$\Delta m$: {}'.format(df['par_s_slope_delta'][0]))
		axes[1,1].text(0.05, 0.6, r'$\Delta b$: {}'.format(df['par_s_offset_delta'][0]))
		axes[1,1].text(0.05, 0.5, r'$\tau$: {}'.format(df['par_s_tau_mean'][0]))
		axes[1,1].text(0.05, 0.4, r'$\Delta \tau$: {}'.format(df['par_s_tau_delta'][0]))
		axes[1,1].text(0.05, 0.3, r'$\Delta B$: {}'.format(df['par_s_boundary_delta'][0]))
		axes[1,1].text(0.05, 0.2, r'$B_N$: {}'.format(df['par_s_initbounds'][0]))
		axes[1,1].text(0.35, 0.8, 'Reg:')
		axes[1,1].text(0.35, 0.7, r'$\lambda$: {}'.format(df['par_r_lambda'][0]))
		axes[1,1].text(0.35, 0.6, r'$m_w$: {}'.format(df['par_r_slope_weight'][0]))
		axes[1,1].text(0.35, 0.5, r'$b_w$: {}'.format(df['par_r_offset_weight'][0]))
		axes[1,1].text(0.35, 0.4, r'$\tau_w$: {}'.format(df['par_r_tau_weight'][0]))
		axes[1,1].text(0.65, 0.8, 'Opt:')
		axes[1,1].text(0.65, 0.7, r'$it$: {}'.format(df['par_o_maxiter'][0]))
		axes[1,1].text(0.65, 0.6, r'$C$: {:.0e}'.format(df['par_o_maxcost'][0]) ) 
		axes[1,1].text(0.65, 0.5, r'$\rho_E$: {:.0e}'.format(df['par_o_rhoend'][0]) )
		axes[1,1].text(0.65, 0.4, r'$S$: {}'.format(df['par_o_earlystop'][0]))
		axes[1,1].text(0.65, 0.3, r'$\epsilon$: {}'.format(df['par_o_epsilon'][0]))
		axes[1,1].text(0.65, 0.2, r'$P$: {}'.format(df['par_o_patience'][0]))
		axes[1,1].axhline(0.15, color='lightgray',ls='--')
		axes[1,1].text(0.05, 0.05, r'$\sigma$: {}'.format(round( df['res_rmse'][0], 2 ) ) )
		axes[1,1].text(0.35, 0.05, r'$\rho$: {}'.format(round( df['res_corr'][0], 2 ) ) )
		axes[1,1].text(0.65, 0.05, r'$t$: {}'.format( round(df['res_time'][0]/1000, 1)) )

		fig.tight_layout()
		filePath = file.rsplit('.',1)[0]
		if '/' in filePath:
			filePath = file.rsplit('/',1)[1]
		plt.savefig('Plot_Demo_{}.pdf'.format(filePath))
		plt.show()
		plt.close()
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def read_LOG( self, file ):
		return pd.read_csv( file, sep='\s*,\s*', engine = 'python')
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def get_command_options( self, args ):
		commands = []

		for arg_list in args.__dict__:
			command = []
			if len( args.__dict__[arg_list] ) != 2:
				raise SystemExit('FATAL ERROR: args passed to the run() function must have the form [ [\"--option\"], [list of arguments] ].')
			for item in args.__dict__[ arg_list ][ 1 ]:
				command.append( [ args.__dict__[ arg_list ][ 0 ], item ] )
			commands.append( command )
		cmd_list = list( itertools.product(*commands) )
		print('Will perform runs with following options: {}'.format( cmd_list ) )
		print('Will perform {} runs with different options.'.format(len(cmd_list)) )
		return cmd_list
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def run( self, files, bounds = None, args = None):

		for fileIndex, file in enumerate( files ):

			filePath = inputPath + file
			CMD_options = []
			if args != None:
				CMD_options.extend( self.get_command_options( args ) )
			else:
				CMD_options.append(1)

			for cmd_option_list in CMD_options:
				CMD = ['--log', '--utterance', str( file ) ]

				if args != None:
					for option in cmd_option_list:
						for e_index, element in enumerate(option):
							#print('Option: {}, is str instance: {}'.format(element, isinstance(option, str) ) )
							if not isinstance(element, str):
								option[ e_index ] = str(element)
						CMD.extend(  option  )

				CMD_TO1 = deepcopy( CMD )
				CMD_BND = deepcopy( CMD )

				CMD_TO1.extend( ['--tg', filePath + '.TextGrid', '--boundaryDelta',"0"] )
				CMD_BND.extend( ['--tg', filePath + '.TextGrid'] )

				CMDS = []
				CMDS.append( CMD_TO1 )
				CMDS.append( CMD_BND )

				if bounds != None:
					assert len(files) == len(bounds), 'Error: Length of file list and length of bounds list do not match.'
					CMD_INI = deepcopy( CMD )
					CMD_INI.extend( ['--initBounds', str( bounds[ fileIndex ] ) ] )
					CMDS.append( CMD_INI )

				for cmd in CMDS:
					file_nr = 0
					while os.path.exists( logPath + 'LOG_{}.csv'.format( file_nr ) ):
						file_nr += 1
					CMD = ['./Sources/TargetOptimizer', filePath + '.PitchTier', logPath + 'LOG_{}.csv'.format( file_nr ),'--log']
					CMD.extend( cmd )
					print(CMD)
					subprocess.call( CMD )

		print('Done.')
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#

#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#

#---------------------------------------------------------------------------------------------------------------------------------------------------#
def main( args ):

	TO_Benchmark = TO_Benchmarks()

	if args.test:
		TO_Benchmark.eval_test()

	if args.demo != None:
		TO_Benchmark.plot_demo( args.demo )

	if args.hyper:
		TO_Benchmark.eval_hyper_hyper()

	return
#---------------------------------------------------------------------------------------------------------------------------------------------------#

#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Description of the TO benchamark class:')
	#parser.add_argument('--utterances', nargs='+', type=str,   help='The utterances to evaluate.')
	#parser.add_argument('--bounds',     nargs='+', type=str,   help='The bounds of the utterances to evaluate.')
	parser.add_argument('--test',       action='store_true',  help='Runs test example.')
	parser.add_argument('--demo',       nargs='?', type= str, const='LOG_Example.csv',  help='Plot demo.')
	parser.add_argument('--hyper',      action='store_true',  help='Runs hyper parameter evaluation on the example samples.')


	args = parser.parse_args()
	main( args )

#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#