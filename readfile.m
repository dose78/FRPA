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
xaxisvals = data{find(strcmp(header, xaxis))};
yaxisvals = data{find(strcmp(header, yaxis))};

if not(isempty(strfind(infile,'single-emerald')))
    if isempty(strfind(folder,'carma'))
        peak = 578.8;
    else
        peak = 209.7;
    end
elseif not(isempty(strfind(infile,'double-emerald')))
    if isempty(strfind(folder,'carma'))
        peak = 289.4;
    else
        peak = 111.104;
    end
elseif not(isempty(strfind(infile,'single-boxboro')))
    if isempty(strfind(folder,'carma'))
        peak = 723.2;
    else
        peak = 252.68;
    end
elseif not(isempty(strfind(infile,'double-boxboro')))
    if isempty(strfind(folder,'carma'))
        peak = 361.6;
    else
        peak = 141.68;
    end
else
    peak = -1;
    display('peak unknown, set to -1');
end

if not(isempty(strfind(algorithm,'STRASSEN')))
    comp = 'BBB';
    xaxislabel = 'Matrix Size (m = k = n, thousands)';
    yaxislabel = 'Effective GFlops';
    peak_label = 'Classical Peak';
    interleavings_plot = strassen_interleavings;
    comp_label = comp;
else
    comp = 'BBBBB';
    xaxislabel = 'Matrix Size (k, thousands)';
    yaxislabel = 'GFlops';
    peak_label = 'Effective Peak';
    interleavings_plot = carma_interleavings;
    comp_label = [comp, ' (Original CARMA)'];
end
