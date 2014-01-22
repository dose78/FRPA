% Config
clear;

infiles = {};

folder = 'data/strassen';
infiles{end+1} = 'strassen-single-boxboro.csv';
infiles{end+1} = 'strassen-double-boxboro.csv';
infiles{end+1} = 'strassen-single-emerald.csv';
infiles{end+1} = 'strassen-double-emerald.csv';

show_legends = true;

xaxis_val = 'k'; % m, k, or n
yaxis_val = 'max'; % max, avg, median, or min

% carma_interleavings_all = {'BBBBBB', 'BBBDBBB', 'BDBBBBB', 'BBBBBDB', 'BBBBDBB', 'BBBBBBD', 'DBBBBBB', 'BBBDDBBB', 'BBDBBDBB', 'BBBBBDDB', 'BBDBDBDBB', 'BDBDBDBDBDB', 'BBBBBBB', 'BBBDBBBB', 'BBBBDBBB', 'BDBBBBBB', 'BBBBBBDB', 'BBBBBBBD', 'DBBBBBBB', 'BBBDBDBBB', 'BBDBBBDBB', 'BDBBBBBDB', 'BBBBBBDDB', 'BBBDBDBDBB', 'BDBDBDBDBDBDB'};
% strassen_interleavings_all = {'BB', 'DBB', 'BDB', 'BBD', 'DBDB', 'BDDB', 'BDBD', 'BBB', 'DBBB', 'BDBB', 'BBDB', 'BBBD', 'BDBDB', 'BBDBD', 'BBBB', 'BDBBB', 'BBDBB', 'BBBDB', 'BDBDBB', 'BBDBDB'};

carma_interleavings = {'BBBBBB', 'BBBBBDB', 'BBBDBBB', 'BBBDDBBB', 'BBDBDBDBB', 'BDBDBDBDBDB', 'BBBBBBB', 'BBBDBBBB', 'BBBBDBBB', 'BBBDBDBBB', 'BBDBBBDBB', 'BBBBBBDDB'};

% GOOD:
strassen_interleavings = {'BB', 'DBB', 'BDB', 'BDBD', 'BBB', 'DBBB', 'BDBB', 'BDBDB'};


tick_label_size = 15;
axis_label_size = 21;
% my_axis = [0 4700 0 4700];
% my_tick = [0:1000:4000];
line_width = 1;

set(0,'DefaultAxesLineStyleOrder', '--|-|:');
% colororder = [0,.5,0; 0,0,1; 1,0,0; 0,.75,.75; .75,0,.75; .75,.75,0; .25,.25,.25];
% colororder = get(gca,'ColorOrder');
% set(0,'DefaultAxesColorOrder', [blue; green; red]);
peak_color = [0,.5,0];

%% ---------------------------- %%

for infile = infiles
    infile = infile{1};
    display(['running ', infile]);
    readfile;
    infilename_orig = infilename;

    y_plot_round = 50.0;

    if not(isempty(strfind(infile,'single-emerald')))
        if not(isempty(strfind(folder,'carma')))
            peak = 213.5;
        else
            y_plot_round = 100.0;
            peak = 578.8;
        end
    elseif not(isempty(strfind(infile,'double-emerald')))
        if not(isempty(strfind(folder,'carma')))
            y_plot_round = 25.0;
            peak = 111.1;
        else
            peak = 289.4;
        end
    elseif not(isempty(strfind(infile,'single-boxboro')))
        if not(isempty(strfind(folder,'carma')))
            peak = 260.86;
        else
            y_plot_round = 100.0;
            peak = 723.2;
        end
    elseif not(isempty(strfind(infile,'double-boxboro')))
        if not(isempty(strfind(folder,'carma')))
            y_plot_round = 25.0;
            peak = 141.68;
        else
            peak = 361.6;
        end
    else
        peak = -1;
        display('peak unknown, set to -1');
    end

    if not(isempty(strfind(algorithm,'STRASSEN')))
        xaxislabel = 'Matrix Size (m = k = n, thousands)';
        yaxislabel = 'Effective GFlops';
        peak_label = 'Classical Peak';
        interleavings_plot = strassen_interleavings;
        frpa_alg_name = 'FRPA Strassen';
        colororder = [0,0,1; 1,0,0; 0,.75,.75; .75,0,.75];
    else
        xaxislabel = 'Middle Dimension k (m = n) (thousands)';
        yaxislabel = 'GFlops';
        peak_label = 'Effective Peak';
        interleavings_plot = carma_interleavings;
        frpa_alg_name = 'FRPA CARMA';
        colororder = [0,0,1; 1,0,0; 0,.75,.75; .75,0,.75; .75,.75,0; .25,.25,.25];
    end

    set(0,'DefaultAxesColorOrder', colororder);

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

    xmatrix_all = [];
    ymatrix_all = [];
    for interleaving = xaxes.keys
        interleaving = interleaving{1};
        xmatrix_all = [xmatrix_all, xaxes(interleaving)'/1000];
        ymatrix_all = [ymatrix_all, yaxes(interleaving)'];
    end


    % SELECTED INTERLEAVINGS
    xmatrix = [];
    ymatrix = [];
    for interleaving = interleavings_plot
        interleaving = interleaving{1};
        xmatrix = [xmatrix, xaxes(interleaving)'/1000];
        ymatrix = [ymatrix, yaxes(interleaving)'];
    end
    max_x = max(max(xmatrix));
    max_y = max([peak, max(max(ymatrix))]);

    fig = figure;
    plot(xmatrix, ymatrix, 'LineWidth', line_width);
    hold on;
    plot([0, max_x], [peak, peak], ':', 'Color', peak_color, 'LineWidth', line_width*1.5);
    hold off;
    interleavings_legend = interleavings_plot;
    interleavings_legend{end+1} = peak_label;
    if show_legends
        lh = legend(interleavings_legend, 'Location', 'southeast');
        legend('boxoff');
    end
    xlabel({'',xaxislabel},'fontsize',axis_label_size);
    ylabel(yaxislabel,'fontsize',axis_label_size);
    max_y_plot = y_plot_round*(ceil(max_y/y_plot_round)); % round up to nearest y_plot_round
    axis([0 max_x 0 max_y_plot]);
    set(gca,'fontsize',tick_label_size,'ytick',[0:y_plot_round:max_y_plot]);
    filename = [infilename_orig, '.eps'];
    print(fig,'-depsc',filename);

    % READ MKL
    infile = strrep(infile, 'strassen', 'mkl');
    infile = strrep(infile, 'carma', 'mkl');
    readfile;

    % OPENTUNER/MKL
    xaxis = xmatrix_all(:,1);
    y_mkl = yaxisvals;
    y_ot = max([ymatrix_all, y_mkl]')';

    max_x = max(max(xaxis));
    max_y = max([peak, max(max(y_ot))]);

    fig = figure;
    plot([0, max(max(xaxis))], [peak, peak], '--', 'Color', peak_color, 'LineWidth', line_width*1.5);
    hold on;
    plot(xaxis, y_ot, 'r-', 'LineWidth', line_width*1.5);
    legend_labels = {peak_label, frpa_alg_name, 'MKL'};
    if not(isempty(strfind(infilename_orig,'carma')))
        if not(isempty(strfind(infilename_orig,'emerald')))
            plot(xaxis, yaxes('BBBBB')', 'r--', 'LineWidth', line_width*1.5);
            legend_labels = {peak_label, frpa_alg_name, 'Original CARMA', 'MKL'};
        end
    end
    plot(xaxis, y_mkl, 'b-', 'LineWidth', line_width*1.5);
    plot(xaxis, y_ot, 'r--', 'LineWidth', line_width*1.5);
    hold off;
    if show_legends
        lh = legend(legend_labels, 'Orientation', 'Horizontal', 'Location', 'NorthOutside');
        legend('boxoff')
    end
    xlabel({'',xaxislabel},'fontsize',axis_label_size);
    ylabel(yaxislabel,'fontsize',axis_label_size);
    max_y_plot = y_plot_round*(ceil(max_y/y_plot_round)); % round up to nearest y_plot_round
    axis([0 max_x 0 max_y_plot]);
    set(gca,'fontsize',tick_label_size,'ytick',[0:y_plot_round:max_y_plot]);
    filename = [infilename_orig, '-ot', '.eps'];
    print(fig,'-depsc',filename);

end
