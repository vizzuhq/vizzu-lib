#include "style.h"

#include "base/refl/auto_struct.h"

namespace Vizzu::Styles
{

const Font &Chart::getDefaultFont()
{
	static const auto instance =
	    Font{.fontFamily = ::Anim::String("Roboto, sans-serif"),
	        .fontStyle = Gfx::Font::Style::normal,
	        .fontWeight = Gfx::Font::Weight::Normal(),
	        .fontSize = Gfx::Length{12}};
	return instance;
}

const Gfx::ColorPalette &Chart::getDefaultColorPalette()
{
	typedef Gfx::Color C;
	static const Gfx::ColorPalette palette{
	    C::RGB(0x4171cd),
	    C::RGB(0x03ae71),
	    C::RGB(0xf4941b),
	    C::RGB(0xf4c204),
	    C::RGB(0xd49664),
	    C::RGB(0xf25456),
	    C::RGB(0x9e67ab),
	    C::RGBA(188, 166, 4),
	    C::RGBA(132, 110, 28),
	    C::RGBA(252, 118, 60),
	    C::RGBA(180, 98, 172),
	    C::RGBA(244, 146, 252),
	    C::RGBA(188, 74, 148),
	    C::RGBA(156, 126, 244),
	    C::RGBA(156, 82, 180),
	    C::RGBA(108, 162, 252),
	    C::RGBA(92, 110, 188),
	    C::RGBA(124, 134, 140),
	    C::RGBA(172, 150, 140),
	    C::RGBA(76, 116, 80),
	    C::RGBA(172, 122, 76),
	    C::RGBA(124, 174, 84),
	    C::RGBA(76, 116, 80),
	    C::RGBA(156, 26, 108),
	    C::RGBA(172, 62, 148),
	    C::RGBA(180, 18, 4),
	};

	return palette;
}

Chart Chart::def()
{
	// clang-format off
	return Chart 
	{
		Padding
		{
			.paddingTop = Gfx::Length::Emphemeral(5.0 / 12.0),
			.paddingRight = Gfx::Length::Emphemeral(5.0 / 12.0),
			.paddingBottom = Gfx::Length::Emphemeral(0.0),
			.paddingLeft = Gfx::Length::Emphemeral(5.0 / 12.0)
		},
		Box
		{
			.backgroundColor = Gfx::Color::White(),
			.borderColor = Gfx::Color::White(),
			.borderWidth = 0,
		},
		Font
		{
			.fontFamily = ::Anim::String("Roboto, sans-serif"),
			.fontStyle = Gfx::Font::Style::normal,
			.fontWeight = Gfx::Font::Weight::Normal(),
			.fontSize = Gfx::Length::Emphemeral(1)
		},
		ChartParams 
		{
			.plot = {
				Padding
				{
					.paddingTop = Gfx::Length::Emphemeral(50.0 / 12.0),
					.paddingRight = Gfx::Length::Emphemeral(45.0 / 12.0),
					.paddingBottom = Gfx::Length::Emphemeral(70.0 / 12.0),
					.paddingLeft = Gfx::Length::Emphemeral(80.0 / 12.0)
				},
				Box
				{
					.backgroundColor = Gfx::Color(),
					.borderColor = Gfx::Color(),
					.borderWidth = 0,
				},
				PlotParams 
				{
					.marker = {
						DataPoint
						{
							.colorGradient = Gfx::ColorGradient(
							{
								{
									0.0,
									Gfx::Color::RGB(0x4171cd)
								},
								{
									0.25,
									Gfx::Color::RGB(0x8536c7)
								},
								{
									0.5,
									Gfx::Color::RGB(0xc33e7f)
								},
								{
									0.75,
									Gfx::Color::RGB(0xe9974e)
								},
								{
									1.0,
									Gfx::Color::RGB(0xf3f239)
								}
							}),
							.colorPalette = ::Anim::Interpolated<Gfx::ColorPalette>(getDefaultColorPalette()),
							.minLightness = -0.4,
							.maxLightness = 0.4,
							.lineMinWidth = 0.001,
							.lineMaxWidth = 0.01,
							.circleMinRadius = 0.005,
							.circleMaxRadius = 0.05,
							.rectangleSpacing = ::Anim::Interpolated<std::optional<double>>(std::nullopt)
						},
						MarkerParams
						{
							.borderWidth = 1,
							.borderOpacity = 1,
							.borderOpacityMode = ::Anim::Interpolated<Marker::BorderOpacityMode>(Marker::BorderOpacityMode::premultiplied),
							.fillOpacity = 1,
							.guides = {
								.color = Gfx::Color::Gray(0.91),
								.lineWidth = 0.5
							},
							.label = {
								OrientedLabel
								{
									Label
									{
										Padding
										{
											.paddingTop = Gfx::Length::Emphemeral(5 / 11.0),
											.paddingRight = Gfx::Length::Emphemeral(5 / 11.0),
											.paddingBottom = Gfx::Length::Emphemeral(5 / 11.0),
											.paddingLeft = Gfx::Length::Emphemeral(5 / 11.0)
										}, 
										Font
										{
											.fontFamily = ::Anim::String(),
											.fontStyle = Gfx::Font::Style::normal,
											.fontWeight = Gfx::Font::Weight::Bold(),
											.fontSize = Gfx::Length::Emphemeral(11.0 / 12.0)
										}, 
										Text
										{
											.color = Gfx::Color(),
											.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::center),
											.backgroundColor = Gfx::Color(),
											.numberFormat = ::Text::NumberFormat::grouped,
											.maxFractionDigits = 3,
											.numberScale = ::Text::NumberScale{}
										}
									},
									OrientedLabelParams
									{
										.orientation = Anim::Interpolated<OrientedLabel::Orientation>(OrientedLabel::Orientation::horizontal),
										.angle = Geom::Angle180()
									}
								},
								MarkerLabelParams
								{
									.position = Anim::Interpolated<MarkerLabel::Position>(MarkerLabel::Position::center),
									.filter = Gfx::ColorTransform::Lightness(0),
									.format = MarkerLabel::Format::measureFirst
								}
							}
						}
					},
					.xAxis = {
						.color = Gfx::Color::Gray(0.8),
						.title = {
							Label
							{
								Padding
								{
									.paddingTop = Gfx::Length::Emphemeral(24 / 14.0),
									.paddingRight = Gfx::Length::Emphemeral(5 / 14.0),
									.paddingBottom = Gfx::Length::Emphemeral(24 / 14.0),
									.paddingLeft = Gfx::Length::Emphemeral(5 / 14.0)
								}, 
								Font
								{
									.fontFamily = ::Anim::String(),
									.fontStyle = Gfx::Font::Style::normal,
									.fontWeight = Gfx::Font::Weight::Normal(),
									.fontSize = Gfx::Length::Emphemeral(14.0 / 12.0)
								}, 
								Text
								{
									.color = Gfx::Color::Gray(0.6),
									.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::left),
									.backgroundColor = Gfx::Color(),
									.numberFormat = ::Text::NumberFormat::prefixed,
									.maxFractionDigits = 3,
									.numberScale = ::Text::NumberScale{}
								}
							},
							AxisTitleParams
							{
								.position = Anim::Interpolated<AxisTitle::Position>(AxisTitle::Position::min_edge),
								.side = Anim::Interpolated<AxisTitle::Side>(AxisTitle::Side::negative),
								.vposition = Anim::Interpolated<AxisTitle::VPosition>(AxisTitle::VPosition::middle),
								.vside = Anim::Interpolated<AxisTitle::VSide>(AxisTitle::VSide::upon),
								.orientation = Anim::Interpolated<AxisTitle::Orientation>(AxisTitle::Orientation::horizontal)
							}
						},
						.label = {
							OrientedLabel
							{
								Label
								{
									Padding
									{
										.paddingTop = Gfx::Length::Emphemeral(8 / 12.0),
										.paddingRight = Gfx::Length::Emphemeral(8 / 12.0),
										.paddingBottom = Gfx::Length::Emphemeral(8 / 12.0),
										.paddingLeft = Gfx::Length::Emphemeral(8 / 12.0)
									},
									Font
									{
										.fontFamily = ::Anim::String(),
										.fontStyle = Gfx::Font::Style::normal,
										.fontWeight = Gfx::Font::Weight::Normal(),
										.fontSize = Gfx::Length::Emphemeral(12.0 / 12.0)
									},
									Text
									{
										.color = Gfx::Color::Gray(0.6),
										.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::left),
										.backgroundColor = Gfx::Color(),
										.numberFormat = ::Text::NumberFormat::prefixed,
										.maxFractionDigits = 3,
										.numberScale = ::Text::NumberScale{}
									}
								},
								OrientedLabelParams
								{
									.orientation = Anim::Interpolated<OrientedLabel::Orientation>(OrientedLabel::Orientation::horizontal),
									.angle = Geom::Angle180()
								}
							},
							AxisLabelParams
							{
								.position = Anim::Interpolated<AxisLabel::Position>(AxisLabel::Position::min_edge),
								.side = Anim::Interpolated<AxisLabel::Side>(AxisLabel::Side::negative)
							}
						},
						.ticks = {
							.color = Gfx::Color::Gray(0.8),
							.lineWidth = 1,
							.length = Gfx::Length::Emphemeral(5.0 / 12.0),
							.position = ::Anim::Interpolated<Tick::Position>(Tick::Position::outside)
						},
						.guides = {
							.color = Gfx::Color::Gray(0.93),
							.lineWidth = 1
						},
						.interlacing = {
							.color = Gfx::Color::Gray(0.97)
						}
					},
					.yAxis = {
						.color = Gfx::Color::Gray(0.8),
						.title = {
							Label
							{
								Padding
								{
									.paddingTop = Gfx::Length::Emphemeral(15 / 14.0),
									.paddingRight = Gfx::Length::Emphemeral(5 / 14.0),
									.paddingBottom = Gfx::Length::Emphemeral(15 / 14.0),
									.paddingLeft = Gfx::Length::Emphemeral(5 / 14.0)
								},
								Font
								{
									.fontFamily = ::Anim::String(),
									.fontStyle = Gfx::Font::Style::normal,
									.fontWeight = Gfx::Font::Weight::Normal(),
									.fontSize = Gfx::Length::Emphemeral(14.0 / 12.0)
								}, 
								Text
								{
									.color = Gfx::Color::Gray(0.6),
									.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::left),
									.backgroundColor = Gfx::Color(),
									.numberFormat = ::Text::NumberFormat::prefixed,
									.maxFractionDigits = 3,
									.numberScale = ::Text::NumberScale{}
								}
							},
							AxisTitleParams
							{
								.position = Anim::Interpolated<AxisTitle::Position>(AxisTitle::Position::min_edge),
								.side = Anim::Interpolated<AxisTitle::Side>(AxisTitle::Side::upon),
								.vposition = Anim::Interpolated<AxisTitle::VPosition>(AxisTitle::VPosition::end),
								.vside = Anim::Interpolated<AxisTitle::VSide>(AxisTitle::VSide::positive),
								.orientation = Anim::Interpolated<AxisTitle::Orientation>(AxisTitle::Orientation::horizontal)
							}
						},
						.label = {
							OrientedLabel
							{
								Label
								{
									Padding
									{
										.paddingTop = Gfx::Length::Emphemeral(8 / 12.0),
										.paddingRight = Gfx::Length::Emphemeral(8 / 12.0),
										.paddingBottom = Gfx::Length::Emphemeral(8 / 12.0),
										.paddingLeft = Gfx::Length::Emphemeral(8 / 12.0)
									},
									Font
									{
										.fontFamily = ::Anim::String(),
										.fontStyle = Gfx::Font::Style::normal,
										.fontWeight = Gfx::Font::Weight::Normal(),
										.fontSize = Gfx::Length::Emphemeral(12.0 / 12.0)
									},
									Text
									{
										.color = Gfx::Color::Gray(0.6),
										.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::left),
										.backgroundColor = Gfx::Color(),
										.numberFormat = ::Text::NumberFormat::prefixed,
										.maxFractionDigits = 3,
										.numberScale = ::Text::NumberScale{}
									}
								},
								OrientedLabelParams
								{
									.orientation = Anim::Interpolated<OrientedLabel::Orientation>(OrientedLabel::Orientation::horizontal),
									.angle = Geom::Angle180(),
								}
							},
							AxisLabelParams
							{
								.position = Anim::Interpolated<AxisLabel::Position>(AxisLabel::Position::min_edge),
								.side = Anim::Interpolated<AxisLabel::Side>(AxisLabel::Side::negative)
							}
						},
						.ticks = {
							.color = Gfx::Color::Gray(0.8),
							.lineWidth = 1,
							.length = Gfx::Length::Emphemeral(5.0 / 12.0),
							.position = ::Anim::Interpolated<Tick::Position>(Tick::Position::outside)
						},
						.guides = {
							.color = Gfx::Color::Gray(0.93),
							.lineWidth = 1
						},
						.interlacing = {
							.color = Gfx::Color::Gray(0.97)
						}
					},
					.areaColor = Gfx::Color::Transparent(),
					.overflow = ::Anim::Interpolated<Overflow>(Overflow::hidden)
				}
			},
			.legend = {
				Padding
				{
					.paddingTop = Gfx::Length::Emphemeral(10.0 / 12.0),
					.paddingRight = Gfx::Length::Emphemeral(5.0 / 12.0),
					.paddingBottom = Gfx::Length::Emphemeral(70.0 / 12.0),
					.paddingLeft = Gfx::Length::Emphemeral(20.0 / 12.0)
				},
				Box
				{
					.backgroundColor = Gfx::Color(),
					.borderColor = Gfx::Color(),
					.borderWidth = 0,
				},
				LegendParams
				{
					.width = Gfx::Length::Emphemeral(100.0 / 12.0),
					.maxWidth = Gfx::Length::Relative(0.3),
					.title = {
						Padding
						{
							.paddingTop = Gfx::Length::Emphemeral(12 / 14.0),
							.paddingRight = Gfx::Length::Emphemeral(5 / 14.0),
							.paddingBottom = Gfx::Length::Emphemeral(5 / 14.0),
							.paddingLeft = Gfx::Length::Emphemeral(5 / 14.0)
						},
						Font
						{
							.fontFamily = ::Anim::String(),
							.fontStyle = Gfx::Font::Style::normal,
							.fontWeight = Gfx::Font::Weight::Normal(),
							.fontSize = Gfx::Length::Emphemeral(14.0 / 12.0)
						},
						Text
						{
							.color = Gfx::Color::Gray(0.6),
							.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::left),
							.backgroundColor = Gfx::Color(),
							.numberFormat = ::Text::NumberFormat::prefixed,
							.maxFractionDigits = 3,
							.numberScale = ::Text::NumberScale{}
						},
					},
					.label = {
						Padding
						{
							.paddingTop = Gfx::Length::Emphemeral(5.0 / 12.0),
							.paddingRight = Gfx::Length::Emphemeral(5 / 12.0),
							.paddingBottom = Gfx::Length::Emphemeral(5 / 12.0),
							.paddingLeft = Gfx::Length::Emphemeral(5 / 12.0)
						},
						Font
						{
							.fontFamily = ::Anim::String(),
							.fontStyle = Gfx::Font::Style::normal,
							.fontWeight = Gfx::Font::Weight::Normal(),
							.fontSize = Gfx::Length::Emphemeral(12.0 / 12.0)
						},
						Text
						{
							.color = Gfx::Color::Gray(0.6),
							.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::left),
							.backgroundColor = Gfx::Color(),
							.numberFormat = ::Text::NumberFormat::prefixed,
							.maxFractionDigits = 3,
							.numberScale = ::Text::NumberScale{}
						},
					},
					.marker = {
						.type = ::Anim::Interpolated<Legend::Marker::Type>(Legend::Marker::Type::circle),
						.size = Gfx::Length::Emphemeral(18.0 / 14.0)
					}
				}
			},
			.title = {
				Padding
				{
					.paddingTop = Gfx::Length::Emphemeral(10.0 / 26.0),
					.paddingRight = Gfx::Length::Emphemeral(10.0 / 26.0),
					.paddingBottom = Gfx::Length::Emphemeral(5.0 / 26.0),
					.paddingLeft = Gfx::Length::Emphemeral(10.0 / 26.0)
				},
				Font
				{
					.fontFamily = ::Anim::String(),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = Gfx::Length::Emphemeral(26.0 / 12.0)
				},
				Text
				{
					.color = Gfx::Color::RGB(0x494949),
					.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::center),
					.backgroundColor = Gfx::Color(),
					.numberFormat = ::Text::NumberFormat::prefixed,
					.maxFractionDigits = 3,
					.numberScale = ::Text::NumberScale{}
				},
			},
			.subtitle = {
				Padding
				{
					.paddingTop = Gfx::Length::Emphemeral(7.0 / 26.0),
					.paddingRight = Gfx::Length::Emphemeral(10.0 / 26.0),
					.paddingBottom = Gfx::Length::Emphemeral(0),
					.paddingLeft = Gfx::Length::Emphemeral(10.0 / 26.0)
				},
				Font
				{
					.fontFamily = ::Anim::String(),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = Gfx::Length::Emphemeral(26.0 / 21.0)
				},
				Text
				{
					.color = Gfx::Color::RGB(0x494949),
					.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::center),
					.backgroundColor = Gfx::Color(),
					.numberFormat = ::Text::NumberFormat::prefixed,
					.maxFractionDigits = 3,
					.numberScale = ::Text::NumberScale{}
				},
			},
			.caption = {
				Padding
				{
					.paddingTop = Gfx::Length::Emphemeral(0.475),
					.paddingRight = Gfx::Length::Emphemeral(1.6 * 30 / 26 + 40.0 * 29 / 26 / 12.13526042),
					.paddingBottom = Gfx::Length::Emphemeral(0.475),
					.paddingLeft = Gfx::Length::Emphemeral(0.8 * 30 / 26)
				},
				Font
				{
					.fontFamily = ::Anim::String(),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = Gfx::Length::Emphemeral(26.0 / 30.0)
				},
				Text
				{
					.color = Gfx::Color::Gray(0.65),
					.textAlign = Anim::Interpolated<Text::TextAlign>(Text::TextAlign::left),
					.backgroundColor = Gfx::Color(),
					.numberFormat = ::Text::NumberFormat::prefixed,
					.maxFractionDigits = 3,
					.numberScale = ::Text::NumberScale{}
				},
			},
			.tooltip = {
				Font
				{
					.fontFamily = ::Anim::String(),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = Gfx::Length{12}
				},
				Box
				{
					.backgroundColor = Gfx::Color::White(),
					.borderColor = Gfx::Color::Gray(0.85),
					.borderWidth = 1,
				},
				TooltipParams
				{
					.layout = Anim::Interpolated<Tooltip::Layout>(Tooltip::Layout::multiLine),
					.color = Gfx::Color::Gray(0.1),
					.shadowColor = Gfx::Color(0, 0, 0, 0.04),
					.borderRadius = 3,
					.dropShadow = 3,
					.arrowSize = 8,
					.distance = 2,
					.seriesName = ::Anim::String("")
				}
			},
			.logo = {
				Padding
				{
					.paddingTop = Gfx::Length::Relative(19./78.),
					.paddingRight = Gfx::Length::Relative(2./9.),
					.paddingBottom = Gfx::Length::Relative(19./78.),
					.paddingLeft = Gfx::Length::Relative(2./9.)
				},
				LogoParams
				{
					.width = Gfx::Length::Emphemeral(40.0 * 2900 / 3000 / 12.13526042),
					.filter = Gfx::ColorTransform::OverrideColor(Gfx::Color::Gray(0.85))
				}
			}
		}
	};

	// clang-format on
}

struct FontParentSetter
{
	Font *parent;
	template <class T>
	    requires(std::is_same_v<Font, T>)
	inline void operator()(T &f) const noexcept
	{
		f.fontParent = parent;
	}

	template <class T>
	inline void operator()(Styles::Param<T> const &) const noexcept
	{}
};

void Chart::setup()
{
	Refl::visit(FontParentSetter{this}, *this);
	fontParent = &getDefaultFont();
}

}