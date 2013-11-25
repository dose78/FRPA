% Config
clear;

infiles = {'strassen-double-emerald.csv'};
% infiles{end+1} = 'strassen-double-boxboro.csv';
% infiles{end+1} = 'strassen-single-emerald.csv';
% infiles{end+1} = 'strassen-single-boxboro.csv';
xaxis = 'k'; % m, k, or n
yaxis = 'max'; % max, avg, median, or min

interleavings_to_kill = {'BBDBDB'};

tick_label_size = 15;
axis_label_size = 21;
my_axis = [0 4700 0 4700];
my_tick = [0:1000:4000];
line_width = 1;

%% ---------------------------- %%

for infile = infiles
    infile = infile{1};
    display(['running ', infile]);
    readfile;
    infilename_orig = infilename;

    % key is interleaving
    xaxes = containers.Map;
    yaxes = containers.Map;
    for i = 1:numlines
        interleaving = interleavings(i);
        interleaving = interleaving{1};
        if (max(ismember(interleavings_to_kill, interleaving)) == 0)
            if not (xaxes.isKey(interleaving))
                xaxes(interleaving) = [];
                yaxes(interleaving) = [];
            end
            xaxes(interleaving) = [xaxes(interleaving), xaxisvals(i)];
            yaxes(interleaving) = [yaxes(interleaving), yaxisvals(i)];
        end
    end

    % key is number of Bs
    xaxes_matrix = containers.Map(1, [1,1; 1,1]); xaxes_matrix.remove(1);
    yaxes_matrix = containers.Map(1, [1,1; 1,1]); yaxes_matrix.remove(1);
    seriesnames = containers.Map(1, [1,1]); seriesnames.remove(1);
    allYs = [];
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

        % for optimal
        allYs = [allYs, transpose(yaxes(interleaving))];
    end
    maxYs = max(allYs')';

    set(0,'DefaultAxesLineStyleOrder', '-|--|:');
    colororder = [0,0,1; 1,0,0; 0,.5,0; 0,.75,.75; .75,0,.75; .75,.75,0; .25,.25,.25];
    set(0,'DefaultAxesColorOrder', colororder);
    % colororder = get(gca,'ColorOrder');
    % set(0,'DefaultAxesColorOrder', [blue; green; red]);

    if not(isempty(strfind(algorithm,'STRASSEN')))
        comp = 'BBB';
        xaxislabel = 'Matrix Size (m = k = n, thousands)';
    else
        comp = 'BBBBBB';
        xaxislabel = 'Matrix Size (k, thousands)';
    end
    if not(isempty(strfind(infile,'single-emerald')))
        peak = 578.8;
    elseif not(isempty(strfind(infile,'double-emerald')))
        peak = 289.4;
    elseif not(isempty(strfind(infile,'single-boxboro')))
        peak = 723.2;
    elseif not(isempty(strfind(infile,'double-boxboro')))
        peak = 361.6;
    else
        peak = 100;
        display('peak unknown, arbitrarily set to 100');
    end

    % ALL INTERLEAVINGS
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

        xlabel({'',xaxislabel},'fontsize',axis_label_size);
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
        filename = [infilename_orig, '-', plotext, '.eps'];
        print(fig,'-depsc',filename);
    end

    % OPENTUNER
    fig = figure;
    xaxisot = xaxes(comp)'/1000;
    plot([xaxisot, xaxisot], [yaxes(comp)', maxYs], 'LineWidth', line_width);
    lh = legend({comp, 'OpenTuner'}, 'Location', 'southeast');
    legend('boxoff')
    xlabel({'',xaxislabel},'fontsize',axis_label_size);
    ylabel('GFlops','fontsize',axis_label_size);
    max_x = max(xaxisot);
    max_y = max(maxYs);
    max_y = 50*(ceil(max_y/50.)); % round up to nearest 50
    axis([0 max_x 0 max_y]);
    % set(gca,'fontsize',tick_label_size,'xtick',[0:2000:max_x]);
    set(gca,'fontsize',tick_label_size);
    filename = [infilename_orig, '-', 'opentuner', '.eps'];
    print(fig,'-depsc',filename);

    % COMPARISON
    fig = figure;
    infile = strrep(infile, 'strassen', 'mkl');
    infile = strrep(infile, 'carma', 'mkl');
    readfile;

    plot([xaxisvals/1000, xaxisot, xaxisot], [yaxisvals, maxYs, ones(size(xaxisot,1),1) * peak], 'LineWidth', line_width);
    lh = legend({'MKL', 'Optimal (OpenTuner)', 'Classical Peak'}, 'Location', 'southeast');
    legend('boxoff')
    xlabel({'',xaxislabel},'fontsize',axis_label_size);
    ylabel('GFlops','fontsize',axis_label_size);
    max_x = max([xaxisvals/1000; xaxisot]);
    max_y = max([yaxisvals; maxYs]);
    max_y = 50*(ceil(max_y/50.)); % round up to nearest 50
    axis([0 max_x 0 max_y]);
    % set(gca,'fontsize',tick_label_size,'xtick',[0:2000:max_x]);
    set(gca,'fontsize',tick_label_size);
    filename = [infilename_orig, '-', 'comparison', '.eps'];
    print(fig,'-depsc',filename);
end
