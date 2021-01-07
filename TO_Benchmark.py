import os
import subprocess
import argparse
from argparse import Namespace
import itertools
from copy import deepcopy

import numpy as np
import pandas as pd 

#import soundfile as sf
try:
	import matplotlib
	import matplotlib.pyplot as plt
	from matplotlib.transforms import Affine2D
	from matplotlib.transforms import blended_transform_factory
	matplotlib_loaded = True
except Exception:
	matplotlib_loaded = False

#plt.rcParams["font.family"] = "Times New Roman"
#SMALL_SIZE = 12
#MEDIUM_SIZE = 17
#BIGGER_SIZE = 22
#plt.rc('font', size=BIGGER_SIZE)          # controls default text sizes
#plt.rc('axes', titlesize=BIGGER_SIZE)     # fontsize of the axes title
#plt.rc('axes', labelsize=BIGGER_SIZE)    # fontsize of the x and y labels
#plt.rc('xtick', labelsize=BIGGER_SIZE)    # fontsize of the tick labels
#plt.rc('ytick', labelsize=BIGGER_SIZE)    # fontsize of the tick labels
#plt.rc('legend', fontsize=BIGGER_SIZE)    # legend fontsize
#plt.rc('figure', titlesize=SMALL_SIZE)  # fontsize of the figure title
#plt.rcParams['text.latex.preamble']=[r"\usepackage{lmodern}"]
nice_fonts = {
"text.usetex": True,
"font.family": "serif",
"font.serif" : "Times New Roman",
"font.size": 20}
matplotlib.rcParams.update(nice_fonts)


example_file_list = ['Aesthetik', 'Akkordeon', 'Analog-digital-Wandler',
                     'Betriebssportgemeinschaft', 'Bratsche', 'Buch',
                     'Dateienunterverzeichnis','Schwarzweiss-Malerei']

#example_file_list = ['Dateienunterverzeichnis','Schwarzweiss-Malerei']

example_bounds_list = [4,5,9,7,3,2,9,6]

#example_bounds_list = [9,6]

test_list = ['Analog-digital-Wandler']

#inputPath  = './Examples/'
inputPath  = './TO_Corpus_Clean/'
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
		#self.run( files = ['Analog-digital-Wandler'], bounds = [9], args = None )
		self.run( files = ['Aesthetik'], bounds = [4], args = None )
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def eval_corpus( self ):
		maxIterations_list = [1000]
		maxCostEvaluations_list = [1e5]
		minRhoEnd_list = [1e-3]
		args = Namespace( maxIterations      = ['--maxIterations', maxIterations_list],
			              maxCostEvaluations = ['--maxCostEvaluations', maxCostEvaluations_list],
			              minRhoEnd          = ['--rhoEnd', minRhoEnd_list],
			            )
		df = pd.read_csv( 'TO_Corpus_Clean.txt', sep=' ' )
		df['boundaries'] = df['syllables'] +1 
		self.run( files = list(df['utterance']), bounds = list(df['boundaries']), args = args )
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
	def plot_analysis( self, file = 'LOG_ALL_FILES.csv', minRhoEnd_list = [1e-3,1e-4,1e-5,1e-6]):
		utterance = 'Dateienunterverzeichnis'

		markers = ['o','*','s','d']
		colors  = ['navy','darkorange','darkturquoise','magenta']
		df = self.read_LOG( file )
		df_AD = df.loc[ (df['utterance'] == utterance ) ]#& (df['par_s_boundary_delta']==100) & (df['par_s_initbounds']!=0) ]

		#print('maxcost: {}, time: {}, fmin: {}'.format(len(maxcost_list), len(time_list), len(fmin_list) ) )


		#print( self.pd_str_to_np( df_AD['res_ftmp_array'][5]) )
		#stop
		#fmins = [np.min(x) for x in [self.pd_str_to_np( df_AD['res_ftmp_array'][i] ) for i in df_AD.index ]  ]
		
		fmin_list = [self.pd_str_to_np( df_AD['res_ftmp_array'][i] ) for i in df_AD.index ]
		#for utterance in example_file_list:
		#	df_AD = df.loc[ (df['utterance'] == utterance ) & (df['par_s_boundary_delta']==100) & (df['par_s_initbounds']!=0) ]
		#	df_rhoEnd_list = [ df_AD.loc[df_AD['par_o_rhoend'] == x ] for  x in minRhoEnd_list ]
		#	maxcost_list = [df['par_o_maxcost']  for df in df_rhoEnd_list ]
		#	time_list    = [df['res_time']  for df in df_rhoEnd_list ]
		#	fmin_list    = [df['res_fmin']  for df in df_rhoEnd_list ]
		#	for index, (maxcost, time) in enumerate( zip( maxcost_list, time_list) ):
		#		plt.scatter(maxcost, time, marker= markers[index], color= colors[index], label= r'$\rho_E$: {}'.format(minRhoEnd_list[index]) )
		#	plt.legend(loc='upper right')
		#	plt.xlabel('maxCostEval')
		#	plt.ylabel('time')
		#	plt.title(utterance)
		#	plt.tight_layout()
		#	#plt.savefig('{}_maxCost_fmin.pdf'.format(utterance) )
		#	plt.show()
		#	plt.close()
		#	#stop
		#stop
		avg_list = [sum(x)/len(x) for x in fmin_list]
		avg_min = np.min(avg_list)
		avg_min_pos = np.argmin(avg_list)
		print('min avg : {} at {}, maxcosteval: {}, rhoEnd: {}'.format(avg_min,avg_min_pos, df_AD['par_o_maxcost'][df_AD.index[avg_min_pos]], df_AD['par_o_rhoend'][df_AD.index[avg_min_pos]] ) )
		for ftmps in fmin_list:
			ftmp_min = 1e6
			fmin = []
			for ftmp in ftmps:
				if ftmp < ftmp_min:
					ftmp_min = ftmp
					fmin.append(ftmp_min)
				else:
					fmin.append( min(fmin) )
			if min(fmin) != 0.0:
				plt.plot( fmin )
			#plt.plot( ftmps )
		plt.show()
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def plot_TO_Graph( self, axis, df, index=None, arrows = False):
		#res_boundaries = self.pd_str_to_np( df['res_boundary_array'][i] )
		#ini_boundaries = self.pd_str_to_np( df['ini_boundary_array'][i] )
		for i in df.index:
			res_boundaries = self.pd_str_to_np( df['res_boundary_array'][i] )
			ini_boundaries = self.pd_str_to_np( df['ini_boundary_array'][i] )
			for boundary in res_boundaries:
				axis.axvline( boundary, ymax=0.9, color = 'lightgray', ls='--', dashes=(5,5), alpha = 0.7)
			for boundary in ini_boundaries:
				axis.axvspan( boundary, boundary, ymin = 0.9, color = 'black', alpha = 0.7)
			slopes     = self.pd_str_to_np( df['res_t_slope_array'][i] )
			offsets    = self.pd_str_to_np( df['res_t_offset_array'][i] )
			boundaries = self.pd_str_to_np( df['res_boundary_array'][i] )
			assert len(slopes) == len(offsets) == len(boundaries)-1, 'FATAl ERROR, the target slopes and offsets and/or boundary sizes do nnot match!'
			for index, slope in enumerate( slopes ):
				print(offsets[index])
				x = [ boundaries[index], boundaries[index+1] ]
				y = [ slopes[index]* (x[0]-boundaries[index]) + offsets[index], slopes[index]* (x[1]-boundaries[index]) + offsets[index] ]
				axis.plot( x, y, color ='lightgray')

			axis.plot( self.pd_str_to_np( df['res_f0_time_array'][i] ), self.pd_str_to_np( df['res_f0_value_array'][i] ), color = 'navy', label='TO Fit' )
			axis.scatter( self.pd_str_to_np( df['ini_f0_time_array'][i] ), self.pd_str_to_np( df['ini_f0_value_array'][i] ), color = 'orange', label='Pitch data')
			if arrows == True:
				tform = blended_transform_factory(axis.transData, axis.transAxes)
				y_min, y_max = axis.get_ylim()
				for res_boundary, ini_boundary  in zip(res_boundaries, ini_boundaries):
					if res_boundary != ini_boundary:
						delta = ini_boundary-res_boundary
						axis.annotate('', xy=(ini_boundary-delta,0.9), xycoords=tform, xytext=(ini_boundary,0.9), textcoords=tform,
						ha='center', va='center',arrowprops=dict(arrowstyle='->') )
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def plot_TO_Wave( self, axis, file, boundaries= np.empty(0), text= None):
		wave, sr = sf.read( file )
		time = np.arange( 0, len(wave) ) / sr
		if boundaries.any():
			y_min, y_max = axis.get_ylim()
			for index, boundary in enumerate(boundaries):
				axis.axvspan( boundary, boundary, ymin = 0.9, color = 'black', alpha = 0.5)
				if text != None and index < len(text):
					x_pos = np.abs(boundaries[index + 1] + boundaries[index]) /2
					axis.text(x_pos,0.5, text[index], ha='center')
		axis.plot( time, wave, color = 'navy', label='Audio signal')
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def plot_essv( self, ):
		file = 'LOG_ALL_FILES.csv'
		rhoEnd = 1e-6
		maxCostEvaluations = 1e6
		df_LOG = self.read_LOG( file ).sort_values(by=['utterance'])
		#print(df_LOG)

		# Plot 1 (TO1 vs optimized boundaries bei Aesthetik)
		utterance = 'Bratsche'

		df_TO1 = df_LOG.loc[ (df_LOG['utterance'] == utterance) & 
		                (df_LOG['par_s_boundary_delta']==0) & 
		                (df_LOG['par_s_initbounds']==0) & 
		                (df_LOG['par_o_maxcost']==maxCostEvaluations) & 
		                (df_LOG['par_o_rhoend']==rhoEnd)  ]

		df_BOUND = df_LOG.loc[ (df_LOG['utterance'] == utterance) & 
		                (df_LOG['par_s_boundary_delta']==100) & 
		                (df_LOG['par_s_initbounds']==0) & 
		                (df_LOG['par_o_maxcost']==maxCostEvaluations) & 
		                (df_LOG['par_o_rhoend']==rhoEnd)  ]

		index = df_TO1.index[0]
		fig, axes = plt.subplots(3,1, sharex= True, gridspec_kw={'hspace': 0}, figsize=(12,6))
		self.plot_TO_Wave(axis= axes[0], file= 'Examples/{}.wav'.format(utterance), 
		                  boundaries= self.pd_str_to_np( df_TO1['ini_boundary_array'][index] ), text= ['bra:','tS@'] )
		self.plot_TO_Graph(axis=axes[1], df= df_TO1, arrows=True)
		self.plot_TO_Graph(axis=axes[2], df= df_BOUND, arrows=True)


		for index, ax in enumerate(axes):
			if index > 0:
				ax.set_ylim(84, 99)
				ax.set_ylabel(r'$f_0$ [St]', labelpad=23)
				tform = blended_transform_factory(ax.transData, ax.transAxes)
				if index == 1:
					ax.annotate('Without boundary\n optimization', xy=(1.269,0.18), xycoords=tform, textcoords=tform, fontsize=15,
							ha='center', va='center')
				if index == 2:
					ax.annotate('With boundary\n optimization', xy=(1.269,0.18), xycoords=tform, textcoords=tform, fontsize=15,
							ha='center', va='center')
			elif index == 0:
				ax.set_ylim(-0.69, 0.69)
				ax.set_ylabel('Amplitude')
			ax.legend(loc='upper right')
		plt.xlabel('Time [s]')
		plt.xlim(0.3, 1.4)

		plt.tight_layout()
		plt.savefig('TO1_vs_BoundaryOPT_ESSV_PLOT_1.pdf')
		#plt.show()
		plt.close()

		# Plot 2 (Peformance measured across utterances)
		df_TO1 = df_LOG.loc[ 
		                (df_LOG['par_s_boundary_delta']==0) & 
		                (df_LOG['par_s_initbounds']==0) & 
		                (df_LOG['par_o_maxcost']==maxCostEvaluations) & 
		                (df_LOG['par_o_rhoend']==rhoEnd)  ]

		df_BOUND = df_LOG.loc[ 
		                (df_LOG['par_s_boundary_delta']==100) & 
		                (df_LOG['par_s_initbounds']==0) & 
		                (df_LOG['par_o_maxcost']==maxCostEvaluations) & 
		                (df_LOG['par_o_rhoend']==rhoEnd)  ]

		df_INIT = df_LOG.loc[ 
		                (df_LOG['par_s_boundary_delta']==100) & 
		                (df_LOG['par_s_initbounds']!=0) & 
		                (df_LOG['par_o_maxcost']==maxCostEvaluations) & 
		                (df_LOG['par_o_rhoend']==rhoEnd)  ]
		df_list = [ df_BOUND, df_INIT ]
		x=[]
		x_err=[]
		for df in df_list:
			fmin_norm = np.array(df['res_fmin']) / np.array(df_TO1['res_fmin'])
			#print(df_TO1)
			#print(df)
			rmse_norm = np.array(df['res_rmse']) / np.array(df_TO1['res_rmse'])
			corr_norm = np.array(df['res_corr']) / np.array(df_TO1['res_corr']) 
			print( 'fmin: {} // {} , RMSE: {} // {}, Corr: {} // {}'.format(fmin_norm.mean(), fmin_norm.std(),
			                                             rmse_norm.mean(), rmse_norm.std(),
			                                             corr_norm.mean(), corr_norm.std() ) )
			x.append([fmin_norm.mean(), rmse_norm.mean(), corr_norm.mean()])
			x_err.append([fmin_norm.std(), rmse_norm.std(), corr_norm.std()])
		df_all_combined = pd.DataFrame(np.array(x).T, index = ['fmin','RMSE','Correlation'], columns=['_a','_b'])
		ax = df_all_combined.plot.barh(alpha= 0.0, legend=False, figsize=(12,4))
		ax.axvline(1.0, color='red', label='TO1')
		print(x[1])
		print([y-0.3 for y in range(0, len(x[0])) ])
		ax.errorbar(x[0],[y-0.1 for y in range(0, len(x[0])) ], xerr= x_err[0], capsize=7, fmt='o', color='black',marker='s', markersize=7, label='TO2 (Annotated)')
		ax.errorbar(x[1],[y+0.1 for y in range(0, len(x[1])) ], xerr= x_err[1], capsize=7, fmt='o', color='black',marker='d', markersize=7, label='TO2 (Uniform)')
		ax.set(xlabel='Relative performance')
		#plt.locator_params(axis='x',nbins=5)
		plt.gca().invert_yaxis()
		plt.legend(loc='upper right')
		plt.xlim(0.2, 1.5)
		plt.tight_layout()
		plt.savefig('stat_vs_performance_ESSV_PLOT_2.pdf')
		plt.show()


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
					#stop

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

	if args.analyse:
		TO_Benchmark.plot_analysis()

	if args.essv:
		TO_Benchmark.plot_essv()

	if args.corpus:
		TO_Benchmark.eval_corpus()

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
	parser.add_argument('--analyse',       nargs='?', type= str, const='LOG_Example.csv',  help='Plot analysis.')
	parser.add_argument('--essv',       nargs='?', type= str, const='LOG_Example.csv',  help='Plot analysis.')
	parser.add_argument('--corpus',      action='store_true',  help='Runs on TargetOptimizer 1.0 corpus.')


	args = parser.parse_args()
	main( args )

#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#