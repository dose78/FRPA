% Config
clear;

infiles = {'strassen-double-emerald.csv'};
% infiles{end+1} = 'strassen-double-boxboro.csv';
% infiles{end+1} = 'strassen-single-emerald.csv';
% infiles{end+1} = 'strassen-single-boxboro.csv';
xaxis = 'k'; % m, k, or n
yaxis = 'max'; % max, avg, median, or min
% show_graph_info = false; % must be true or false (no quotes).

tick_label_size = 15;
axis_label_size = 21;
my_axis = [0 4700 0 4700];
my_tick = [0:1000:4000];
line_width = 1;

%% ---------------------------- %%

for infile = infiles
    infile = infile{1};
    display(['running ', infile]);
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

    interleavings = data{interleaving_i};
    numlines = length(data{1});
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
    seriesnames = containers.Map(1, [1,1]); seriesnames.remove(1);
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

        % all lines on same plot
        if (xaxes_matrix.isKey(-1))
            xaxes_matrix(-1) = [xaxes_matrix(-1), transpose(xaxes(interleaving))];
            yaxes_matrix(-1) = [yaxes_matrix(-1), transpose(yaxes(interleaving))];
            tempseriesnames = seriesnames(-1);
            tempseriesnames{end+1} = interleaving;
            seriesnames(-1) = tempseriesnames;
        else
            xaxes_matrix(-1) = [transpose(xaxes(interleaving))];
            yaxes_matrix(-1) = [transpose(yaxes(interleaving))];
            seriesnames(-1) = {interleaving};
        end
    end

    set(0,'DefaultAxesLineStyleOrder', '-|--|:');
    % set(0,'DefaultAxesColorOrder', [blue; green; red]);
    for numBs = xaxes_matrix.keys
        numBs = numBs{1};
        fig = figure;
        plot(xaxes_matrix(numBs)/1000, yaxes_matrix(numBs), 'LineWidth', line_width);

        num_lines = length(seriesnames(numBs));
        if num_lines < 14
            lh = legend(seriesnames(numBs), 'Location', 'southeast');
        else
            lh = legend(seriesnames(numBs), 'Location', 'northeastoutside');
            % pos = get(lh,'position');
            % set(lh, 'position',[1 0.25 pos(3:4)])
        end
        legend('boxoff')

        xlabel({'','Matrix Size (thousands)'},'fontsize',axis_label_size);
        ylabel('GFlops','fontsize',axis_label_size);
        max_x = max(max(xaxes_matrix(numBs)/1000));
        max_y = max(max(yaxes_matrix(numBs)));
        max_y = 50*(ceil(max_y/50.)); % round up to nearest 50
        axis([0 max_x 0 max_y]);
        % set(gca,'fontsize',tick_label_size,'xtick',[0:2000:max_x]);
        set(gca,'fontsize',tick_label_size);
        if numBs == -1
            plotext = 'all';
        else
            plotext = [num2str(numBs), 'Bs'];
        end
        filename = [infilename, '-', plotext, '.eps'];
        print(fig,'-depsc',filename);
    end
end
