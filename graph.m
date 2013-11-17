% Config
clear;

infile = 'strassen-double-emerald-out.csv';
xaxis = 'k'; % m, k, or n
yaxis = 'max'; % max, avg, median, or min
% show_graph_info = false; % must be true or false (no quotes).

tick_label_size = 15;
axis_label_size = 21;
my_axis = [0 4700 0 4700];
my_tick = [0:1000:4000];
dot_size = 50;
small_dot_size = 18;
linewidth = 3;

red = [1,0,0];
green = [0,1,0];
blue = [0,0,1];
orange = [0.85,0.35,0];

%% ---------------------------- %%

[pathstr,infilename,ext] = fileparts(infile);
fileID = fopen(infile,'r');
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

algorithms = data{algorithm_i};
ms = data{m_i};
ks = data{k_i};
ns = data{n_i};
interleavings = data{interleaving_i};
maxs = data{max_i};
avgs = data{avg_i};
medians = data{median_i};
mins = data{min_i};
stddevs = data{stddev_i};
numtrialss = data{numtrials_i};
numlines = length(algorithms);

xaxisvals = data{find(strcmp(header, xaxis))};
yaxisvals = data{find(strcmp(header, yaxis))};

% key is interleaving
xaxes = containers.Map;
yaxes = containers.Map;

for i = 1:numlines
    interleaving = interleavings(i);
    interleaving = interleaving{1};
    if not (xaxes.isKey(interleaving))
        xaxes(interleaving) = [];
        yaxes(interleaving) = [];
    end
    xaxes(interleaving) = [xaxes(interleaving), xaxisvals(i)];
    yaxes(interleaving) = [yaxes(interleaving), yaxisvals(i)];
end

% key is number of Bs
xaxes_matrix = containers.Map(1, [1,1; 1,1]); xaxes_matrix.remove(1);
yaxes_matrix = containers.Map(1, [1,1; 1,1]); yaxes_matrix.remove(1);
seriesnames = containers.Map(1, [1,1]);
seriesnames.remove(1);
for interleaving = xaxes.keys
    interleaving= interleaving{1};
    numBs = length(find(interleaving=='B'));
    if (xaxes_matrix.isKey(numBs))
        xaxes_matrix(numBs) = [xaxes_matrix(numBs), transpose(xaxes(interleaving))];
        yaxes_matrix(numBs) = [yaxes_matrix(numBs), transpose(yaxes(interleaving))];
        tempseriesnames = seriesnames(numBs);
        tempseriesnames{end+1} = interleaving;
        seriesnames(numBs) = tempseriesnames;
    else
        xaxes_matrix(numBs) = [transpose(xaxes(interleaving))];
        yaxes_matrix(numBs) = [transpose(yaxes(interleaving))];
        seriesnames(numBs) = {interleaving};
    end
end

for numBs = xaxes_matrix.keys
    numBs = numBs{1};
    fig = figure;
    % plot(xaxes_matrix(numBs), yaxes_matrix(numBs), linewidth); linewidth doesn't work
    plot(xaxes_matrix(numBs), yaxes_matrix(numBs));
    lh = legend(seriesnames(numBs));
    % set(lh,'location','northeastoutside');
    % pos = get(lh,'position');
    % set(lh, 'position',[0.8 0.5 pos(3:4)])
    xlabel({'','Matrix Size'},'fontsize',axis_label_size);
    ylabel('GFlops','fontsize',axis_label_size);
    % axis([0 1000 0 100]);
    % pbaspect([1 0.97 1]);
    % set(gca,'fontsize',tick_label_size,'xtick',[0:100:1000],'ytick',[0:10:100]);
    set(gca,'fontsize',tick_label_size);
    filename = [infilename, '-', num2str(numBs), 'Bs', '.eps'];
    print(fig,'-depsc',filename);
end