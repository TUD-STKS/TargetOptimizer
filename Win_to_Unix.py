import pandas as pd
# replacement strings
WINDOWS_LINE_ENDING = b'\r\n'
UNIX_LINE_ENDING = b'\n'

# relative or absolute file path, e.g.:
#file_path = "Zeitdruck.TextGrid"

df = pd.read_csv( 'TO_Corpus_Clean.txt', sep=' ' )

for utterance in list(df['utterance']):
	file_path = 'TO_Corpus_Clean/{}.TextGrid'.format(utterance)
	with open(file_path, 'rb') as open_file:
	    content = open_file.read()
	content = content.replace(WINDOWS_LINE_ENDING, UNIX_LINE_ENDING)
	with open(file_path, 'wb') as open_file:
	    open_file.write(content)