#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#	- This file is a Python wrapper for the commandline interface of the software TargetOptimizer 2.0,
#	  see https://github.com/TUD-STKS/TargetOptimizer
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#
#	- Copyright (C) 2021, Paul Konstantin Krug, Dresden, Germany
#	- https://github.com/TUD-STKS/TargetOptimizer
#	- Author: Paul Konstantin Krug, TU Dresden
#
#	- License:
#
#		This program is free software: you can redistribute it and/or modify
#		it under the terms of the GNU General Public License as published by
#		the Free Software Foundation, either version 3 of the License, or
#		(at your option) any later version.
#		
#		This program is distributed in the hope that it will be useful,
#		but WITHOUT ANY WARRANTY; without even the implied warranty of
#		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#		GNU General Public License for more details.
#		
#		You should have received a copy of the GNU General Public License
#		along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#
# Requirements:
#	- python 3 (tested with version 3.7)
#
# Optional, used for visualization:
#	- matplotlib        (tested with version 3.3.3)
#	- numpy    (tested with version 1.19.5)
#	- pandas   (tested with version 1.2.1)
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#
# Load essential modules:
import os, subprocess
# Try to load some non-essential packages, used for the visualization of TargetOptimizer results:
try:
	import matplotlib
	import matplotlib.pyplot as plt
	import pandas as pd
	import numpy as np
	visualization = True
except ImportError:
	visualization = False
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################



#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#



data_dir = './Examples/'
res_dir  = './Results/'
if not os.path.exists( res_dir ):
	os.mkdir( res_dir )



#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################



#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#
def TargetOptimizer( utterance, number_targets = None, early_stopping = False ):
	CMD=[
	'./TargetOptimizer',
	'{}{}'.format( data_dir, utterance + '.PitchTier' ),
	'--log','{}{}_nt_{}_es_{}.txt'.format( res_dir, utterance, number_targets, early_stopping ),
	'--g', '{}{}_nt_{}_es_{}{}'.format( res_dir, utterance, number_targets, early_stopping, '.ges' ),
	'--c', '{}{}_nt_{}_es_{}{}'.format( res_dir, utterance, number_targets, early_stopping, '.csv' ),
	'--p', '{}{}_nt_{}_es_{}{}'.format( res_dir, utterance, number_targets, early_stopping, '-tam.PitchTier' ),
	'--maxIterations', '1000',
	'--maxCostEvaluations', '1e5',
	'--rhoEnd', '1e-3',
	'--utterance', str( utterance )
	]
	if number_targets != None and isinstance( number_targets, int ):
		CMD.extend( ['--boundaryDelta', '100', '--initBounds', str( int(number_targets + 1) )] )
	else:
		CMD.extend( ['--tg', '{}{}'.format( data_dir, utterance + '.TextGrid' )] )
	if early_stopping:
		CMD.extend( ['--useEarlyStopping'] )
	print( CMD )
	subprocess.call( CMD )
	return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
def pd_str_to_np( df_entry ):
	return np.array( df_entry.split(' ') ).astype(np.float)
#---------------------------------------------------------------------------------------------------------------------------------------------------#
def read_LOG( file ):
	return pd.read_csv( file, sep='\s*,\s*', engine = 'python')
#---------------------------------------------------------------------------------------------------------------------------------------------------#
def plot_demo( log_path):
	assert visualization, 'ERROR in "plot_demo()": The necessary modules for visualization could not be loaded. Are they installed?'
	assert os.path.exists( log_path ), 'ERROR in "plot_demo()": the specified file {:} does not exist.'
	df = read_LOG( log_path )
	fig, axes = plt.subplots(2,2)
	ftmp = pd_str_to_np( df['res_ftmp_array'][0] )
	axes[0,0].plot(    ftmp[ ftmp > -1 ] )
	axes[0,1].plot(    pd_str_to_np( df['res_f0_time_array'][0] ), pd_str_to_np( df['res_f0_value_array'][0] ), color = 'navy' )
	axes[0,1].scatter( pd_str_to_np( df['ini_f0_time_array'][0] ), pd_str_to_np( df['ini_f0_value_array'][0] ), color = 'orange')
	for boundary in pd_str_to_np( df['res_boundary_array'][0] ):
		axes[0,1].axvspan( boundary, boundary, ymax=0.9, color = 'lightgray', ls='--', alpha = 0.5)
	for boundary in pd_str_to_np( df['ini_boundary_array'][0] ):
		axes[0,1].axvspan( boundary, boundary, ymin = 0.9, color = 'black', alpha = 0.5)
	slopes     = pd_str_to_np( df['res_t_slope_array'][0] )
	offsets    = pd_str_to_np( df['res_t_offset_array'][0] )
	boundaries = pd_str_to_np( df['res_boundary_array'][0] )
	assert len(slopes) == len(offsets) == len(boundaries)-1, 'FATAl ERROR, the target slopes and offsets and/or boundary sizes do nnot match!'
	for index, slope in enumerate( slopes ):
		x = [ boundaries[index], boundaries[index+1] ]
		y = [ slopes[index]* (x[0]-boundaries[index]) + offsets[index], slopes[index]* (x[1]-boundaries[index]) + offsets[index] ]
		axes[0,1].plot( x, y, color ='lightgray')
	ax_labels = [ [['Iteration', 'ftmp'], ['Time [s]', 'F0 [st]']], [['Iteration', 'fmin'], ['Iteration', 'fmin']] ]
	for i, axs in enumerate( axes ):
		for j, ax in enumerate( axs ):
			ax.set( xlabel = ax_labels[i][j][0], ylabel = ax_labels[i][j][1] )
	ftmp_min = 1e6
	fmin = []
	for ftmp in pd_str_to_np( df['res_ftmp_array'][0] ):
		if ftmp < ftmp_min:
			ftmp_min = ftmp
			fmin.append(ftmp_min)
		else:
			fmin.append( min(fmin) )
	fmin = np.array( fmin )
	axes[1,0].plot( fmin[ fmin > -1 ] )
	axes[1,1].get_xaxis().set_visible( False )
	axes[1,1].get_yaxis().set_visible( False )
	axes[1,1].text(0.05, 0.9, 'file: {}'.format( df['utterance'][0] ) )
	axes[1,1].text(0.05, 0.8, 'SS:')
	axes[1,1].text(0.05, 0.7, r'$\Delta m$: {}'.format( df['par_s_slope_delta'][0]) )
	axes[1,1].text(0.05, 0.6, r'$\Delta b$: {}'.format( df['par_s_offset_delta'][0]) )
	axes[1,1].text(0.05, 0.5, r'$\tau$: {}'.format( df['par_s_tau_mean'][0]) )
	axes[1,1].text(0.05, 0.4, r'$\Delta \tau$: {}'.format( df['par_s_tau_delta'][0]) )
	axes[1,1].text(0.05, 0.3, r'$\Delta B$: {}'.format( df['par_s_boundary_delta'][0]) )
	axes[1,1].text(0.05, 0.2, r'$B_N$: {}'.format( df['par_s_initbounds'][0]) )
	axes[1,1].text(0.35, 0.8, 'Reg:')
	axes[1,1].text(0.35, 0.7, r'$\lambda$: {}'.format( df['par_r_lambda'][0]) )
	axes[1,1].text(0.35, 0.6, r'$m_w$: {}'.format( df['par_r_slope_weight'][0]) )
	axes[1,1].text(0.35, 0.5, r'$b_w$: {}'.format( df['par_r_offset_weight'][0]) )
	axes[1,1].text(0.35, 0.4, r'$\tau_w$: {}'.format( df['par_r_tau_weight'][0]) )
	axes[1,1].text(0.65, 0.8, 'Opt:')
	axes[1,1].text(0.65, 0.7, r'$it$: {}'.format( df['par_o_maxiter'][0]) )
	axes[1,1].text(0.65, 0.6, r'$C$: {:.0e}'.format( df['par_o_maxcost'][0]) ) 
	axes[1,1].text(0.65, 0.5, r'$\rho_E$: {:.0e}'.format( df['par_o_rhoend'][0]) )
	axes[1,1].text(0.65, 0.4, r'$S$: {}'.format( df['par_o_earlystop'][0]) )
	axes[1,1].text(0.65, 0.3, r'$\epsilon$: {}'.format( df['par_o_epsilon'][0]) )
	axes[1,1].text(0.65, 0.2, r'$P$: {}'.format( df['par_o_patience'][0]) )
	axes[1,1].axhline(0.15, color='lightgray',ls='--')
	axes[1,1].text(0.05, 0.05, r'$\sigma$: {}'.format( round( df['res_rmse'][0], 2 ) ) )
	axes[1,1].text(0.35, 0.05, r'$\rho$: {}'.format( round( df['res_corr'][0], 2 ) ) )
	axes[1,1].text(0.65, 0.05, r'$t$: {}'.format( round( df['res_time'][0]/1000, 1) ) )
	fig.tight_layout()
	plt.savefig( log_path.replace( '.txt', '.pdf' ) )
	plt.show()
	plt.close()
	return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################



#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#
if __name__ == "__main__":
	TargetOptimizer( 'Dateienunterverzeichnis', early_stopping = True )
	TargetOptimizer( 'Dateienunterverzeichnis', number_targets = 8, early_stopping = True )
	plot_demo( res_dir + 'Dateienunterverzeichnis_nt_8_es_True.txt' )
	plot_demo( res_dir + 'Dateienunterverzeichnis_nt_None_es_True.txt' ) 
print('Done.')
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################