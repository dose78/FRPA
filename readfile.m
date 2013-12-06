infile2 = strcat(folder, '/', infile);
[pathstr,infilename,ext] = fileparts(infile2);
fileID = fopen(infile2,'r');
header = textscan(fileID, '%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,\n]', 1);
header = [header{:}];
data = textscan(fileID, '%s%f%f%f%s%f%f%f%f%f%f%*[^\n]', 'Delimiter', ',', 'headerLines', 1);
fclose(fileID);

algorithm_i = find(strcmp(header, 'algorithm'));
m_i = find(strcmp(header, 'm'));
k_i = find(strcmp(header, 'k'));
n_i = find(strcmp(header, 'n'));
interleaving_i =  find(strcmp(header, 'interleaving'));
max_i = find(strcmp(header, 'max'));
avg_i = find(strcmp(header, 'avg'));
median_i = find(strcmp(header, 'median'));
min_i = find(strcmp(header, 'min'));
stddev_i = find(strcmp(header, 'stddev'));
numtrials_i = find(strcmp(header, 'numtrials'));

interleavings = data{interleaving_i};
algorithm = data{algorithm_i}(1);
algorithm = algorithm{1};
numlines = length(data{1});
xaxisvals = data{find(strcmp(header, xaxis_val))};
yaxisvals = data{find(strcmp(header, yaxis_val))};
