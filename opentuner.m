clear;


peak = 124.9512;
y_plot_round = 10;

legend_font_size = 17;
tick_label_size = 15;
axis_label_size = 21;
% my_axis = [0 4700 0 4700];
% my_tick = [0:1000:4000];
line_width = 1.5;
peak_color = [0,.5,0];

infiles = {};
allFiles = dir('data/opentuner/');
for k = 1 : length(allFiles)
    if(strfind(allFiles(k).name, '.csv'))
        if(strfind(allFiles(k).name, 'opentuner'))
            infiles{end+1} = ['data/opentuner/', allFiles(k).name];
        end
    end
end

for infile = infiles
    infile = infile{1};
    [pathstr,infilename,ext] = fileparts(infile);
    display(['running ', infilename]);

    if not(isempty(strfind(infilename,'carma')))
        yaxis_label = 'GFlops';
    else
        yaxis_label = 'Effective GFlops';
    end

    data = csvread(infile);
    num_cols_to_add = 100 - size(data,2);
    data = [data, zeros(size(data,1), num_cols_to_add)];
    data_adjusted = [];
    for i = 1:size(data,1)
        row = data(i,:);
        cur_max = row(1);
        row_adjusted = [cur_max];
        for j = 2:length(row)
            cur_max = max([cur_max, row(j)]);
            row_adjusted = [row_adjusted, cur_max];
        end
        data_adjusted = [data_adjusted; row_adjusted];
    end

    Xs = 1:100;
    Ys = median(data_adjusted);
    first_quart = quantile(data_adjusted,.25);
    third_quart = quantile(data_adjusted,.75);
    peak = max(max(data_adjusted));

    fig = figure;
    plot([0, max(Xs)], [peak, peak], '--', 'Color', peak_color, 'LineWidth', line_width);
    hold on;
    plot(Xs, Ys, 'b', 'LineWidth', line_width);
    errorbar(Xs, Ys, Ys - first_quart, third_quart - Ys);

    xlabel({'','Iteration'},'fontsize',axis_label_size);
    ylabel(yaxis_label,'fontsize',axis_label_size);

    lh = legend({'Maximum Achievable Performance', 'Average Cumulative Performance'}, 'Location', 'southeast');
    set(lh,'FontSize',legend_font_size);
    legend('boxoff');

    max_y_plot = y_plot_round*(ceil(peak/y_plot_round)); % round up to nearest y_plot_round

    axis([0 max(Xs) 0 max_y_plot]);
    % set(gca,'fontsize',tick_label_size,'xtick',[0:2000:max_x]);

    set(gca,'fontsize',tick_label_size);
    print(fig,'-depsc', [infilename, '.eps']);

end
