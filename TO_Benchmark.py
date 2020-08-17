import os
import subprocess
import argparse

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.transforms import Affine2D
import pandas as pd 


example_list = ['Aesthetik', 'Akkordeon', 'Analog-digital-Wandler',
                'Betriebssportgemeinschaft', 'Bratsche',  
                'Dateienunterverzeichnis','Schwarzweiss-Malerei']

bounds_list = [4,5,9,7,3,9,6]

test_list = ['Aesthetik']

inputPath  = './Examples/'
outputPath = './Benchmark/'
logPath    = './LOG/'




#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#
class TO_Benchamrks():
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	"""A class for TO_Benchamrks"""
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def __init__( self, args ):
		self.args = args
		if ( not os.path.exists( logPath ) ):
			os.mkdir( logPath )
		if ( not self.args.files ):
				self.args.files = example_list
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def get_bin( self, value, x_min, steps):
		return round((value - x_min )/steps )
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def get_average_from_last( self, df_list, column ):
		avg = 0
		for df in df_list:
			avg += df.loc[df.index[-1],column]
		return avg / len(df_list)
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def get_values_from_runs( self, df_list, column ):
		last = []
		for df in df_list:
			last.append(df.loc[df.index[-1],column])
		return pd.DataFrame(last)
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def get_weighted_average_and_error_from_last( self, df_list_list, column, bins = 10 ):
		x_min = 1e6
		x_max = 0
		for df_list in df_list_list:
			for df in df_list:
				if df[column].min() < x_min:
					x_min = df[column].min()
				if df[column].max() > x_max:
					x_max = df[column].max()
		#if x_max > 5:
		#	x_max = 5
		#if x_min < 0:
		#	x_min = 0
		binning = []
		steps = (x_max-x_min)/bins 
		for i in range (0, bins+1):
			binning.append( ( x_min - (steps/2) ) + steps * i)



		spreads = []
		weighted_averages = []
		for df_list in df_list_list:
			values = self.get_values_from_runs( df_list, column )
			spreads.append( [values.iloc[:,0].min(), values.iloc[:,0].max()] )
			#print(values)
			weights = values.iloc[:,0].value_counts(bins=binning, sort = False)
			#print(weights)
			weighted_avg = 0	
			sum_of_weights = 0
			weights_list =[]
			for weight in weights:
				weights_list.append(weight)
			#print(weights_list)
			for index, value in enumerate(values.iloc[:,0]):
				#print('value: {} bin: {},'.format(value,self.get_bin(value, x_min, steps)))
				if (self.get_bin(value, x_min, steps) >=10):
					weight = 0
				else:
					weight = weights_list[self.get_bin(value, x_min, steps)]
				if value >= 5:
				 weight = 0
				#print('value: {} bin: {}, weight: {}'.format(value,self.get_bin(value, x_min, steps),weight))
				weighted_avg += weight * value
				sum_of_weights += weight
			weighted_avg /= sum_of_weights
			weighted_averages.append( weighted_avg )


		return weighted_averages, spreads
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def plot_benchmark( self, y_list, y_err_list, outputName ="name"): #RMSE and Time
		y_list = np.array(y_list).T
		y_err_list = np.array(y_err_list).T
		print(y_list)
		print(y_err_list)
		fig, ax = plt.subplots()
		trans1 = Affine2D().translate(-0.2, 0.0) + ax.transData
		trans2 = Affine2D().translate(+0.2, 0.0) + ax.transData
		#trans2 = Affine2D().translate(0.0, 0.0) + ax.transData
		ax.vlines( example_list, y_err_list[0][0] ,y_err_list[1][0], transform=trans1 )
		ax.vlines( example_list, y_err_list[0][1] ,y_err_list[1][1] )
		ax.vlines( example_list, y_err_list[0][2] ,y_err_list[1][2], transform=trans2 )

		ax.plot( example_list, y_list[0], marker="o", linestyle="none", transform=trans1 , label = 'TO1')
		ax.plot(example_list,  y_list[1], marker="o", linestyle="none", label = 'BOUND')
		ax.plot(example_list,  y_list[2], marker= "o", linestyle="none", transform=trans2, label= 'INIT')

		#ax.vline( example_list, y_err[0][0] ,y_err[0][1] )
		#er1 = ax.errorbar(example_list, y[0], yerr = np.array([[y_err[0][0] ,y_err[0][1]]]).T, marker="o", linestyle="none", transform=trans1)
		#er2 = ax.errorbar(example_list, y[1], yerr = np.array([[y_err[1][0] ,y_err[1][1]]]).T, marker="o", linestyle="none")
		#e3 = ax.errorbar(example_list, y[2], yerr = np.array([[y_err[2][0] ,y_err[2][1]]]).T, marker= "o", linestyle="none", transform=trans2)
		plt.xticks(rotation=45, ha="right")
		plt.gcf().subplots_adjust(bottom=0.4)
		plt.ylim(0,2)
		plt.ylabel("MSE")
		#plt.title(Title)
		plt.legend(loc='upper right')
		plt.show()
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def plot_performance( self ):
		file_mse_avg  = []
		file_time_avg = []
		y_list =[]
		y_err_list=[]

		t_list =[]
		t_err_list=[]
		for file in self.args.files:
			df_list = []
			df_list_BOUND = []
			df_list_INIT = []
			for index in range(0, self.args.iterations):
				print(file + " " + str(index))
				df_list.append( pd.read_csv( logPath + 'LOG_{}_RUN_{}.txt'.format( file, index ), sep = ' ') )
				df_list_BOUND.append( pd.read_csv( logPath + 'LOG_{}_RUN_{}_BOUND.txt'.format( file, index ), sep = ' ') )
				df_list_INIT.append( pd.read_csv( logPath + 'LOG_{}_RUN_{}_INIT.txt'.format( file, index ), sep = ' ') )

			self.plot_dfs( df_list )
			self.plot_dfs( df_list_BOUND )
			self.plot_dfs( df_list_INIT )

			y, y_err = self.get_weighted_average_and_error_from_last( [df_list, df_list_BOUND, df_list_INIT], "tmpMSE")
			y_list.append(y)
			y_err_list.append(y_err)

			#t, t_err = self.get_weighted_average_and_error_from_last( [df_list, df_list_BOUND, df_list_INIT], "time")
			#t_list.append(t)
			#t_err_list.append(t_err)
			#self.plot_benchmark( y, y_err )

			#min_mse_avg       = self.get_average_from_last( df_list, "tmpMSE")
			#min_mse_avg_bound = self.get_average_from_last( df_list_BOUND, "tmpMSE")
			#min_mse_avg_init  = self.get_average_from_last( df_list_INIT, "tmpMSE")
			#max_time_avg       = self.get_average_from_last( df_list, "time")
			#max_time_avg_bound = self.get_average_from_last( df_list_BOUND, "time")
			#max_time_avg_init  = self.get_average_from_last( df_list_INIT, "time")

			#file_mse_avg.append( [min_mse_avg, min_mse_avg_bound, min_mse_avg_init] )
			#file_time_avg.append( [max_time_avg, max_time_avg_bound, max_time_avg_init] )



		self.plot_benchmark( y_list, y_err_list, "name")
		#self.plot_benchmark( t_list, t_err_list, "name")
		#elf.plot_benchmark( file_time_avg,"name" )
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def plot_dfs( self, df_list, X_Label = 'Time [s]', Y_Label = 'Costfunction', Title = 'Costfunction'):
		for index, df in enumerate(df_list):
		#for index, _ in enumerate(df.columns):
		#	if labels == None:
		#		LABEL = index
		#	else:
		#		LABEL = labels[index]
			plt.scatter( df["time"]/1000, df['fmin'], label = 'RUN {}'.format(index) )#,linestyle="",marker="o")
		plt.xlabel(X_Label)
		plt.ylabel(Y_Label)
		plt.title(Title)
		plt.legend(loc='upper right')
		plt.savefig('Test.pdf',bbox_inches='tight')
		#plt.show()
		plt.close()
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
	def run( self ):
		#os.chdir( './Sources/' )

		for fileIndex, file in enumerate(self.args.files):
			print('bounds: {}'.format( bounds_list[fileIndex] ))
			filePath = inputPath + file
			for index in range(0, self.args.iterations):
				subprocess.call( ['./TargetOptimizer_CMD', filePath + '.PitchTier', logPath + 'LOG_{}_RUN_{}'.format( file, index ), '--tg', filePath + '.TextGrid', '--boundaryDelta',"0"] )
				subprocess.call( ['./TargetOptimizer_CMD', filePath + '.PitchTier', logPath + 'LOG_{}_RUN_{}_BOUND'.format( file, index ), '--tg', filePath + '.TextGrid'] )
				subprocess.call( ['./TargetOptimizer_CMD', filePath + '.PitchTier', logPath + 'LOG_{}_RUN_{}_INIT'.format( file, index ), '--initBounds', str(bounds_list[ fileIndex ]) ] )
		print('Done.')
		return
#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################



#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Description of the TO benchamark class:')
	parser.add_argument('--files', nargs='+', type=str,   help='The files to evaluate.')
	parser.add_argument('--iterations', type=int,  help='The iterations for each file.')
	parser.add_argument('--outFilePaths', nargs='+', type=str, help='The produced plot will be written to ~/Output/ [Argument]')
	args = parser.parse_args()
	TO_Benchmark = TO_Benchamrks( args )
	#TO_Benchmark.run(  )
	TO_Benchmark.plot_performance()

#---------------------------------------------------------------------------------------------------------------------------------------------------#
#####################################################################################################################################################
#---------------------------------------------------------------------------------------------------------------------------------------------------#