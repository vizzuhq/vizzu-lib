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
		'cartesian/marimekko_rectangle_2dis_2con.mjs': { tags: [m(2), d(2), Co, Di, PW, Ra, r,] },
		'cartesian/stacked_mekko_rectangle_2dis_2con.mjs': { tags: [m(1), d(2), Co, Di, PW, Ra, r,] },
		'cartesian/scatterplot_circle_negative_2dis_3con.mjs': { tags: [m(3), d(2), Re, Di, N, c,] },
		'cartesian/stream_stacked_area_3dis_1con.mjs': { tags: [m(1), d(3), Co, PW, CT, De, OD, a,] },
		'cartesian/area_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, CT, De, N, OD, a,] },
		'cartesian/stacked_area_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, a,] },
//		'cartesian/waterfall_rectangle_negative_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, CT, N, OD, r,] },
		'cartesian/column_grouped_rectangle_negative_2dis_1con.mjs': { tags: [m(1), d(2), Co, Ra, CT, De, N, OD, r,] },
		'cartesian/bar_rectangle_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, Ra, CT, De, N, OD, r,] },
		'cartesian/line_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, CT, De, N, OD, l,] },
		'cartesian/scatterplot_circle_negative_1dis_2con.mjs': { tags: [m(2), d(1), Re, Di, N, c,] },
		'cartesian/dotplot_circle_negative_1dis_1con.mjs': { tags: [m(1), d(1), Di, N, c,] },
		'cartesian/column_stacked_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Di, PW, Ra, r,] },
		'cartesian/line_negative_2dis_1con.mjs': { tags: [m(1), d(2), Co, CT, De, N, OD, l,] },
		'cartesian/column_stacked_rectangle_negative_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, OD, r,] },
		'cartesian/histogram_rectangle_negative_1dis_1con.mjs': { tags: [m(1), d(1), Co, Ra, CT, De, N, OD,] },
		'polar/donut_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), PW, Ra, r,] },
		'polar/radial_stacked_rectangle_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, Ra, r,] },
		'polar/radial_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Co, Ra, r,] },
		'polar/coxcomb_stacked_rectangle_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, OD, r,] },
		'polar/pie_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Co, PW, Ra, r,] },
		'without/treemap_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), Co, PW, r,] },
		'without/bubble_circle_2dis_2con.mjs': { tags: [m(2), d(2), Co, PW, c,] },
		'without/bubble_circle_1dis_2con.mjs': { tags: [m(2), d(1), Co, PW, c,] },
		'without/treemap_rectangle_2dis_2con.mjs': { tags: [m(2), d(2), Co, PW, r,] },	
	},
	templates:
	{
		'composition_comparison_pie_coxcomb_column_2dis_2con.mjs': { tags: [m(2), d(2), Co, PW, Ra, r,] },
		'composition_comparison_waterfall_column_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, De, N, r,] },
		'composition_percentage_area_stream_3dis_1con.mjs': { tags: [m(1), d(2), Co, Di, PW, CT, OD, a,] },
		'composition_percentage_column_stream_3dis_1con.mjs': { tags: [m(1), d(2), Co, Di, PW, CT, OD, r,] },
		'distribution_relationship_dotplot_dotplot.mjs': { tags: [m(2), d(2), Re, Di, N, c,] },
		'drilldown_aggregate_line.mjs': { tags: [m(1), d(2), Co, PW, Ra, N, l,] },
		'merge_split_area_stream_3dis_1con.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, a,] },
		'merge_split_bar.mjs': { tags: [m(1), d(2), Co, PW, r,] },
		'merge_split_radial_stacked_rectangle_2dis_1con.mjs': { tags: [m(1), d(2), Co, PW, r,] },
		'orientation_dot_circle.mjs': { tags: [m(2), d(2), Di, CT, N, OD, c,] },
		'orientation_circle.mjs': { tags: [m(2), d(2), Re, Di, CT, N, OD, c,] },
		'orientation_marimekko_rectangle_2dis_2con.mjs': { tags: [m(2), d(2), Di, PW, r,] },
		'orientation_rectangle.mjs': { tags: [m(1), d(2), Co, PW, CT, N, OD, r,] },
		'pie_donut2_rectangle_1dis_1con.mjs': { tags: [m(1), d(1), PW,] },
		'relationship_comparison_circle_2_bubble_plot.mjs': { tags: [m(2), d(2), Re, Co, Di, N, c,] },
		'relationship_total_bubble_plot_column.mjs': { tags: [m(2), d(2), Re, Di, CT, N, OD, r, c,] },
		'stack_group_area_line.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, N, OD, a, l,] },
		'stack_group_circle.mjs': { tags: [m(1), d(2), Co, PW, c,] },
		'stack_group_treemap.mjs': { tags: [m(1), d(2), Co, PW, r,] },
		'total_element_bubble_2_bar.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, r, c,] },
		'total_element_bubble_column.mjs': { tags: [m(1), d(2), Co, PW, CT, OD, r, c,] },
		'total_time_area_bar.mjs': { tags: [m(1), d(2), Co, PW, CT, N, OD, r, a,] },
		'total_time_area_column.mjs': { tags: [m(1), d(2), Co, PW, CT, N, OD, r, a,] },
		'treemap_radial.mjs': { tags: [m(1), d(2), Co, PW,] },
		'zoom_area.mjs': { tags: [m(1), d(2), Co, PW, CT, N, OD, a,] },
		'zoom_line.mjs': { tags: [m(1), d(2), Co, PW, Ra, CT, N, OD, l,] },	
	}
};

module.exports = exampleList;
