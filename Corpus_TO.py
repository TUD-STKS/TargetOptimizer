import numpy
import os
import pandas
import math
from shutil import copyfile

############################################
# sort out missing files:
df = pandas.read_csv( 'TO_Corpus_ORIG.txt', sep=' ' )
print(df)
for index in df.index:
    if os.path.exists( 'TO_Corpus_Clean/{}.PitchTier'.format(df['utterance'][index]) ) and os.path.exists( 'TO_Corpus_Clean/{}.TextGrid'.format(df['utterance'][index]) ):
        print('exists')
    else:
        print(df['utterance'][index])
        df = df.drop(index)
print(df)
#stop
df.to_csv('TO_Corpus_Clean.txt', sep = ' ', index = None)
stop
############################################

def getIntegerPlaces(theNumber):
    if theNumber <= 999999999999997:
        return int(math.log10(theNumber)) + 1
    else:
        counter = 15
        while theNumber >= 10**counter:
            counter += 1
        return counter

df = pandas.read_csv('Corpus_TO_Sampa/corpus.sampa', sep='\s*,\s*', engine = 'python')
boundaries = [getIntegerPlaces(x) for x in df['pattern']]
df['boundaries'] = boundaries
df = df.sort_values(by=['utterance'])
for index in df.index:
    if ( any( i.isdigit() for i in df['utterance'][index] ) ) or ('_var' in df['utterance'][index]) :
        df = df.drop(index)
    if os.path.exists( 'Corpus_TO/{}.PitchTier'.format(df['utterance'][index]) ) and os.path.exists( 'Corpus_TO/{}.TextGrid'.format(df['utterance'][index]) ):
        chyrs=0
    else:
        df = df.drop(index)

df = df.drop(columns=['sampa', 'pattern'])


for utterance in df['utterance']:
    if os.path.exists( 'Corpus_TO/{}.PitchTier'.format(utterance) ) and os.path.exists( 'Corpus_TO/{}.TextGrid'.format(utterance) ):
        copyfile( 'Corpus_TO/{}.PitchTier'.format(utterance), 'TO_Corpus_Clean/{}.PitchTier'.format(utterance) )
        copyfile( 'Corpus_TO/{}.TextGrid'.format(utterance), 'TO_Corpus_Clean/{}.TextGrid'.format(utterance) )

df.to_csv('TO_Corpus_Clean.txt', sep = ' ', index = None)
print(df)





