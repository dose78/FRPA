clear;

L3_SIZE = 24;

show_legend = false;

y_plot_round = 10000;

legend_font_size = 17;
tick_label_size = 15;
axis_label_size = 21;
line_width = 2;
peak_color = [0,.5,0];
L3_size_color = [.75,0,.75];

infile = 'stream_misses-avg.csv';
[pathstr,infilename,ext] = fileparts(infile);
display(['running ', infilename]);

data = csvread(infile);
data = [0, 0; data];

Xs = data(:,1);
Xs_o = data(:,1);
Ys = data(:,2);

Xs = Xs*8; % convert to bytes
Xs = Xs/(1024*1024); % convert to MB
Xs = Xs*2; % two arrays

max_y_expected = max(Xs)*1024*1024/64;

fig = figure;
hold on;
plot([0, max(Xs)], [0, max_y_expected], '--', 'Color', peak_color, 'LineWidth', line_width);
plot(Xs, Ys, 'b', 'LineWidth', line_width);

plot([L3_SIZE, L3_SIZE], [0, max([Ys; max_y_expected])], '-.', 'Color', L3_size_color, 'LineWidth', line_width);

xlabel({'','2*Array Size (MB)'},'fontsize',axis_label_size);
ylabel('L3 Cache Misses','fontsize',axis_label_size);

if show_legend
    lh = legend({'Expected Cache Misses for 2*(Array Size) > L3 Cache Size', 'Reported L3 Cache Misses', 'L3 Cache Size'}, 'Location', 'southoutside');
    set(lh,'FontSize',legend_font_size);
    legend('boxoff');
end

max_y_plot = y_plot_round*(ceil(max([Ys; max_y_expected])/y_plot_round)); % round up to nearest y_plot_round
% axis([0 max(Xs) 0 max_y_plot]);
axis([0 100 0 100*1024*1024/64]);

set(gca,'fontsize',tick_label_size,'xtick',[0:10:100]);
% set(gca,'fontsize',tick_label_size);

 print(fig,'-depsc', [infilename, '.eps']);

