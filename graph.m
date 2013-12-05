% Config
clear;

infiles = {};

folder = 'data/carma';
% infiles{end+1} = 'carma-double-emerald.csv';
% infiles{end+1} = 'carma-double-boxboro.csv';
% infiles{end+1} = 'carma-single-emerald.csv';
% infiles{end+1} = 'carma-single-boxboro.csv';
xaxis = 'k'; % m, k, or n
yaxis = 'max'; % max, avg, median, or min

carma_interleavings = {'BBBBBB', 'BBBBBBD', 'BBBBBDB', 'BBBBBDDB', 'BBBBDBB', 'BBBDBBB', 'BBBDDBBB', 'BBDBBDBB', 'BBDBDBDBB', 'BDBBBBB', 'BDBDBDBDBDB', 'DBBBBBB', 'BBBBBBB', 'BBBDBBBB', 'BBBBDBBB', 'BDBBBBBB', 'BBBBBBDB', 'BBBBBBBD', 'DBBBBBBB', 'BBBDBDBBB', 'BBDBBBDBB', 'BDBBBBBDB', 'BBBBBBDDB', 'BBBDBDBDBB', 'BDBDBDBDBDBDB'};
strassen_interleavings = {'BBBBBB', 'BBBBBBB'};

y_plot_round = 50.0;

tick_label_size = 15;
axis_label_size = 21;
% my_axis = [0 4700 0 4700];
% my_tick = [0:1000:4000];
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
        if not (xaxes.isKey(interleaving))
            xaxes(interleaving) = [];
            yaxes(interleaving) = [];
        end
        xaxes(interleaving) = [xaxes(interleaving), xaxisvals(i)];
        yaxes(interleaving) = [yaxes(interleaving), yaxisvals(i)];
    end

    set(0,'DefaultAxesLineStyleOrder', '-|--|:');
    % colororder = [0,.5,0; 0,0,1; 1,0,0; 0,.75,.75; .75,0,.75; .75,.75,0; .25,.25,.25];
    peak_color = [0,.5,0];
    colororder = [0,0,1; 1,0,0; 0,.75,.75; .75,0,.75; .75,.75,0; .25,.25,.25];
    set(0,'DefaultAxesColorOrder', colororder);
    % colororder = get(gca,'ColorOrder');
    % set(0,'DefaultAxesColorOrder', [blue; green; red]);


    % ALL INTERLEAVINGS
    fig = figure;
    xmatrix = [];
    ymatrix = [];
    for interleaving = interleavings_plot
        interleaving = interleaving{1};
        xmatrix = [xmatrix, xaxes(interleaving)'/1000];
        ymatrix = [ymatrix, yaxes(interleaving)'];
    end
    max_x = max(max(xmatrix));
    max_y = max([peak, max(max(ymatrix))]);

    plot(xmatrix, ymatrix, 'LineWidth', line_width);
    hold on;
    plot([0, max_x], [peak, peak], 'Color', peak_color, 'LineWidth', line_width);
    hold off;

    interleavings_legend = interleavings_plot;
    interleavings_legend{end+1} = peak_label;
    lh = legend(interleavings_legend, 'Location', 'southeast');
    % lh = legend(tempseriesnames, 'Location', 'northeastoutside');
    % pos = get(lh,'position');
    % set(lh, 'position',[1 0.25 pos(3:4)])
    legend('boxoff')

    xlabel({'',xaxislabel},'fontsize',axis_label_size);
    ylabel(yaxislabel,'fontsize',axis_label_size);
    max_y_plot = y_plot_round*(ceil(max_y/y_plot_round)); % round up to nearest y_plot_round
    axis([0 max_x 0 max_y_plot]);
    set(gca,'fontsize',tick_label_size);
    % set(gca,'fontsize',tick_label_size,'xtick',[0:2000:max_x]);
    filename = [infilename_orig, '.eps'];
    print(fig,'-depsc',filename);


    % OPENTUNER
    fig = figure;
    xmatrix_ot = [xmatrix(:,1), xmatrix(:,1)];
    ymatrix_ot = [yaxes(comp)', max(ymatrix')'];
    max_x = max(max(xmatrix_ot));
    max_y = max([peak, max(max(ymatrix_ot))]);

    plot(xmatrix_ot, ymatrix_ot, 'LineWidth', line_width);
    hold on;
    plot([0, max(max(xmatrix_ot))], [peak, peak], 'Color', peak_color, 'LineWidth', line_width);
    hold off;
    lh = legend({comp_label, 'OpenTuner', peak_label}, 'Location', 'southeast');
    legend('boxoff')
    xlabel({'',xaxislabel},'fontsize',axis_label_size);
    ylabel(yaxislabel,'fontsize',axis_label_size);
    max_y_plot = y_plot_round*(ceil(max_y/y_plot_round)); % round up to nearest y_plot_round
    axis([0 max_x 0 max_y_plot]);
    % set(gca,'fontsize',tick_label_size,'xtick',[0:2000:max_x]);
    set(gca,'fontsize',tick_label_size);
    filename = [infilename_orig, '-', 'opentuner', '.eps'];
    print(fig,'-depsc',filename);


    % MKL COMPARISON
    fig = figure;
    infile = strrep(infile, 'strassen', 'mkl');
    infile = strrep(infile, 'carma', 'mkl');
    readfile;

    xmatrix_comp = [xaxisvals/1000, xmatrix(:,1)];
    ymatrix_comp = [yaxisvals, max(ymatrix')'];
    max_x = max(max(xmatrix_comp));
    max_y = max([peak, max(max(ymatrix_comp))]);

    plot([xaxisvals/1000, xmatrix(:,1)], [yaxisvals, max(ymatrix')'], 'LineWidth', line_width);
    hold on;
    plot([0, max(xaxisvals/1000)], [peak, peak], 'Color', peak_color, 'LineWidth', line_width);
    hold off;
    lh = legend({'MKL', 'Optimal (OpenTuner)', peak_label}, 'Location', 'southeast');
    legend('boxoff')
    xlabel({'',xaxislabel},'fontsize',axis_label_size);
    ylabel(yaxislabel,'fontsize',axis_label_size);
    max_y_plot = y_plot_round*(ceil(max_y/y_plot_round)); % round up to nearest y_plot_round
    axis([0 max_x 0 max_y_plot]);
    % set(gca,'fontsize',tick_label_size,'xtick',[0:2000:max_x]);
    set(gca,'fontsize',tick_label_size);
    filename = [infilename_orig, '-', 'comparison', '.eps'];
    print(fig,'-depsc',filename);

end
