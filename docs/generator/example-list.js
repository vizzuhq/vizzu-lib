const m = (cnt) => cnt + ' measure' + (cnt > 1) ? 's' : '';
const d = (cnt) => cnt + ' dimension' + (cnt > 1) ? 's' : '';
const Co = 'Comparison';
const Di = 'Distribution';
const PW = 'Part-to-whole';
const Ra = 'Ranking';
const Re = 'Relationship';
const CT = 'Change over time';
const De = 'Deviation';
const N = 'Negative values';
const OD = 'Ordered dimension';
const l = 'Line';
const r = 'Rectangle';
const a = 'Area';
const c = 'Circle';

const exampleList =
{
	sample_static:
	{
		'cartesian/histogram_rectangle_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, Ra, CT, De, N, OD, ] },
		'cartesian/column_grouped_rectangle_negative_2dis_1con.mjs': { tags: [m(1), d(2), Co, Ra, CT, De, N, OD, r, ] },
		'cartesian/waterfall_rectangle_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, PW, CT, De, N, OD, r, ] },
		'cartesian/column_stacked_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Di, PW, Ra, r, ] },
		'cartesian/column_stacked_rectangle_negative_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, OD, r, ] },
		'cartesian/bar_rectangle_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, Ra, CT, De, N, OD, r, ] },
		'cartesian/stacked_mekko_rectangle_2dis_2con.mjs': { tags: [m(1), d(2), Co, Di, PW, Ra, r, ] },
		'cartesian/mekko_rectangle_1dis_2con.mjs': { tags: [m(2), d(1), Co, PW, ] },
		'cartesian/marimekko_rectangle_2dis_2con.mjs': { tags: [m(2), d(2), Co, Di, PW, Ra, r, ] },
		'polar/pie_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Co, PW, Ra, r, ] },
		'polar/donut_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), PW, Ra, r, ] },
		'polar/coxcomb_stacked_rectangle_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, OD, r, ] },
		'polar/radial_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Co, Ra, r, ] },
		'polar/radial_stacked_rectangle_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, Ra, r, ] },
		'without/treemap_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Co, PW, r, ] },
		'without/treemap_rectangle_2dis_2con.mjs': { tags: [m(2), d(2), Co, PW, r, ] },
		'cartesian/line_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, CT, De, N, OD, l, ] },
		'cartesian/line_negative_2dis_1con.mjs': { tags: [m(1), d(2), Co, CT, De, N, OD, l, ] },
		'polar/spiderweb_line_1dis_1con.mjs': { tags: [m(1), d(1), Co, CT, N, OD, l, ] },
		'polar/NO_spiderweb_line_2dis_1con.mjs': { tags: [m(1), d(2), Co, CT, N, OD, l, ] },
		'cartesian/area_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, CT, De, N, OD, a, ] },
		'cartesian/stacked_area_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, a, ] },
		'cartesian/stream_stacked_area_3dis_1con.mjs': { tags: [m(1), d(3), Co, PW, CT, OD, a, ] },
		'polar/spiderweb_area_1dis_1con.mjs': { tags: [m(1), d(1), Co, CT, N, OD, a, ] },
		'cartesian/dotplot_circle_negative_1dis_1con.mjs': { tags: [m(1), d(1), Di, N, c, ] },
		'cartesian/scatterplot_circle_negative_1dis_2con.mjs': { tags: [m(2), d(1), Re, Di, N, c, ] },
		'cartesian/scatterplot_circle_negative_2dis_3con.mjs': { tags: [m(3), d(2), Re, Di, N, c, ] },
		'without/bubble_circle_1dis_2con.mjs': { tags: [m(2), d(1), Co, PW, c, ] },
		'without/bubble_circle_2dis_2con.mjs': { tags: [m(2), d(2), Co, PW, c, ] },
	},
	templates:
	{
		'composition_percentage_area_stream_3dis_1con.mjs': { tags: [m(1), d(2), Co, Di, PW, CT, OD, a, ], desc: 'This vizzu is great to show the distribution of categorical data over time: the total value, the share of components and components side-by-side.\nExamples of use:\n* Number of goods sold over time\n* Change of population in the countries of a continent\n* No. of bugs in software versions or components' },
		'composition_comparison_pie_coxcomb_column_2dis_2con.mjs': { tags: [m(2), d(2), Co, PW, Ra, r, ] },
		'composition_percentage_column_stream_3dis_1con.mjs': { tags: [m(1), d(2), Co, Di, PW, CT, OD, r, ] },
		'merge_split_area_stream_3dis_1con.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, a, ] },
		'total_element_bubble_2_bar.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, r, c, ] },
		'merge_split_bar.mjs': { tags: [m(1), d(2), Co, PW, r, ] },
		'merge_split_radial_stacked_rectangle_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, r, ] },
		'orientation_circle.mjs': { tags: [m(2), d(2), Re, Di, CT, N, OD, c, ] },
		'orientation_marimekko_rectangle_2dis_2con.mjs': { tags: [m(2), d(2), Di, PW, r, ] },
		'pie_donut2_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), PW, ] },
		'relationship_comparison_circle_2_bubble_plot.mjs': { tags: [m(2), d(2), Re, Co, Di, N, c, ] },
		'relationship_total_bubble_plot_column.mjs': { tags: [m(2), d(2), Re, Di, CT, N, OD, r, c, ] },
		'stack_group_area_line.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, N, OD, a, l, ] },
		'stack_group_circle.mjs': { tags: [m(1), d(2), Co, PW, c, ] },
		'stack_group_treemap.mjs': { tags: [m(1), d(2), Co, PW, r, ] },
		'total_element_bubble_column.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, r, c, ] },
		'total_time_area_column.mjs': { tags: [m(1), d(2), Co, PW, CT, N, OD, r, a, ] },
		'treemap_radial.mjs': { tags: [m(1), d(2), Co, PW, ] },
		'zoom_area.mjs': { tags: [m(1), d(2), Co, PW, CT, N, OD, a, ] },
		'composition_percentage_column_3dis_1con.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, OD, r, ] },
		'total_time_bar_line.mjs': { tags: [m(1), d(2), Co, Ra, CT, OD, r, l, ] },
	}
};

module.exports = exampleList;
