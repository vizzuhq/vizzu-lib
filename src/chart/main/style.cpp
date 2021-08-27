#include "style.h"

#include "chart/rendering/palettes.h"

using namespace Vizzu;
using namespace Vizzu::Styles;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-designator"
#endif

Font Chart::defaultFont{
	.fontFamily = ::Anim::String("Roboto, sans-serif"),
	.fontStyle = Gfx::Font::Style::normal,
	.fontWeight = Gfx::Font::Weight::Normal(),
	.fontSize = Gfx::Length(12)
};

Chart Chart::def()
{
	return {
		{
			.paddingTop = Gfx::Length(),
			.paddingRight = Gfx::Length(),
			.paddingBottom = Gfx::Length(),
			.paddingLeft = Gfx::Length()
		},
		{
			.backgroundColor = Gfx::Color::White(),
			.borderColor = Gfx::Color::White(),
			.borderWidth = 0,
		},
		{
			.fontFamily = ::Anim::String("Roboto, sans-serif"),
			.fontStyle = Gfx::Font::Style::normal,
			.fontWeight = Gfx::Font::Weight::Normal(),
			.fontSize = Gfx::Length::Emphemeral(1)
		},
		.plot = {
			{
				.paddingTop = Gfx::Length::Emphemeral(50.0/12.0),
				.paddingRight = Gfx::Length::Emphemeral(50.0/12.0),
				.paddingBottom = Gfx::Length::Emphemeral(70.0/12.0),
				.paddingLeft = Gfx::Length::Emphemeral(70.0/12.0)
			},
			{
				.backgroundColor = Gfx::Color(),
				.borderColor = Gfx::Color(),
				.borderWidth = 0,
			},
			.marker = {
				.borderWidth = 1,
				.borderOpacity = 0.5,
				.borderOpacityMode =
					::Anim::Interpolated<Marker::BorderOpacityMode>
					(Marker::BorderOpacityMode::premultiplied),
				.fillOpacity = 1,
				.guides = {
					.color = Gfx::Color::Gray(0.91),
					.lineWidth = 0.5
				},
				.label = {
					{
						{
							{
								.paddingTop = Gfx::Length::Emphemeral(5/11.0),
								.paddingRight = Gfx::Length::Emphemeral(5/11.0),
								.paddingBottom = Gfx::Length::Emphemeral(5/11.0),
								.paddingLeft = Gfx::Length::Emphemeral(5/11.0)
							},
							{
								.fontFamily = ::Anim::String(),
								.fontStyle = Gfx::Font::Style::normal,
								.fontWeight = Gfx::Font::Weight::Bold(),
								.fontSize = Gfx::Length::Emphemeral(11.0/12.0)
							},
							{
								.color = Param<Gfx::Color>(),
								.textAlign = Anim::Interpolated<Text::TextAlign>
									(Text::TextAlign::center),
								.backgroundColor = Gfx::Color(),
								.numberFormat = ::Text::NumberFormat::grouped
							}
						},
						.orientation = Anim::Interpolated
							<OrientedLabel::Orientation>
							(OrientedLabel::Orientation::horizontal),
						.angle = 0,
					},
					.position = Anim::Interpolated<MarkerLabel::Position>
						(MarkerLabel::Position::center),
					.filter = Gfx::ColorTransform::Lightness(0),
					.format = MarkerLabel::Format::valueFirst
				}
			},
			.xAxis = {
				.color = Gfx::Color::Gray(0.8),
				.title = {
					{
						{
							.paddingTop = Gfx::Length::Emphemeral(20/14.0),
							.paddingRight = Gfx::Length::Emphemeral(5/14.0),
							.paddingBottom = Gfx::Length::Emphemeral(5/14.0),
							.paddingLeft = Gfx::Length::Emphemeral(5/14.0)
						},
						{
							.fontFamily = ::Anim::String(),
							.fontStyle = Gfx::Font::Style::normal,
							.fontWeight = Gfx::Font::Weight::Normal(),
							.fontSize = Gfx::Length::Emphemeral(14.0/12.0)
						},
						{
							.color = Gfx::Color::Gray(0.6),
							.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::left),
							.backgroundColor = Gfx::Color(),
							.numberFormat = ::Text::NumberFormat::prefixed
						}
					},
					.position = Anim::Interpolated<AxisTitle::Position>
						(AxisTitle::Position::min_edge),
					.side = Anim::Interpolated<AxisTitle::Side>
						(AxisTitle::Side::negative),
					.vposition = Anim::Interpolated<AxisTitle::VPosition>
						(AxisTitle::VPosition::middle),
					.vside = Anim::Interpolated<AxisTitle::VSide>
						(AxisTitle::VSide::upon),
					.orientation = Anim::Interpolated<AxisTitle::Orientation>
						(AxisTitle::Orientation::horizontal)
				},
				.label = {
					{
						{
							{
								.paddingTop = Gfx::Length::Emphemeral(5/12.0),
								.paddingRight = Gfx::Length::Emphemeral(5/12.0),
								.paddingBottom = Gfx::Length::Emphemeral(5/12.0),
								.paddingLeft = Gfx::Length::Emphemeral(5/12.0)
							},
							{
								.fontFamily = ::Anim::String(),
								.fontStyle = Gfx::Font::Style::normal,
								.fontWeight = Gfx::Font::Weight::Normal(),
								.fontSize = Gfx::Length::Emphemeral(12.0/12.0)
							},
							{
								.color = Gfx::Color::Gray(0.6),
								.textAlign = Anim::Interpolated<Text::TextAlign>
										(Text::TextAlign::left),
								.backgroundColor = Gfx::Color(),
								.numberFormat = ::Text::NumberFormat::prefixed
							}
						},
						.orientation = Anim::Interpolated
							<OrientedLabel::Orientation>
							(OrientedLabel::Orientation::horizontal),
						.angle = 0,
					},
					.position = Anim::Interpolated<AxisLabel::Position>
						(AxisLabel::Position::min_edge),
					.side = Anim::Interpolated<AxisLabel::Side>
						(AxisLabel::Side::negative)
				},
				.ticks = {
					.color = Gfx::Color::Gray(0.8),
					.lineWidth = 1,
					.length = Gfx::Length::Emphemeral(5.0/12.0),
					.position = ::Anim::Interpolated<Tick::Position>
						(Tick::Position::outside)
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
					{
						{
							.paddingTop = Gfx::Length::Emphemeral(5/14.0),
							.paddingRight = Gfx::Length::Emphemeral(5/14.0),
							.paddingBottom = Gfx::Length::Emphemeral(15/14.0),
							.paddingLeft = Gfx::Length::Emphemeral(5/14.0)
						},
						{
							.fontFamily = ::Anim::String(),
							.fontStyle = Gfx::Font::Style::normal,
							.fontWeight = Gfx::Font::Weight::Normal(),
							.fontSize = Gfx::Length::Emphemeral(14.0/12.0)
						},
						{
							.color = Gfx::Color::Gray(0.6),
							.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::left),
							.backgroundColor = Gfx::Color(),
			 				.numberFormat = ::Text::NumberFormat::prefixed
						}
					},
					.position = Anim::Interpolated<AxisTitle::Position>
						(AxisTitle::Position::min_edge),
					.side = Anim::Interpolated<AxisTitle::Side>
						(AxisTitle::Side::upon),
					.vposition = Anim::Interpolated<AxisTitle::VPosition>
						(AxisTitle::VPosition::end),
					.vside = Anim::Interpolated<AxisTitle::VSide>
						(AxisTitle::VSide::positive),
					.orientation = Anim::Interpolated<AxisTitle::Orientation>
						(AxisTitle::Orientation::horizontal)
				},
				.label = {
					{
						{
							{
								.paddingTop = Gfx::Length::Emphemeral(5/12.0),
								.paddingRight = Gfx::Length::Emphemeral(5/12.0),
								.paddingBottom = Gfx::Length::Emphemeral(5/12.0),
								.paddingLeft = Gfx::Length::Emphemeral(5/12.0)
							},
							{
								.fontFamily = ::Anim::String(),
								.fontStyle = Gfx::Font::Style::normal,
								.fontWeight = Gfx::Font::Weight::Normal(),
								.fontSize = Gfx::Length::Emphemeral(12.0/12.0)
							},
							{
								.color = Gfx::Color::Gray(0.6),
								.textAlign = Anim::Interpolated<Text::TextAlign>
										(Text::TextAlign::left),
								.backgroundColor = Gfx::Color(),
								.numberFormat = ::Text::NumberFormat::prefixed
							}
						},
						.orientation = Anim::Interpolated
							<OrientedLabel::Orientation>
							(OrientedLabel::Orientation::horizontal),
						.angle = 0,
					},
					.position = Anim::Interpolated<AxisLabel::Position>
						(AxisLabel::Position::min_edge),
					.side = Anim::Interpolated<AxisLabel::Side>
						(AxisLabel::Side::negative)
				},
				.ticks = {
					.color = Gfx::Color::Gray(0.8),
					.lineWidth = 1,
					.length = Gfx::Length::Emphemeral(5.0/12.0),
					.position = ::Anim::Interpolated<Tick::Position>
						(Tick::Position::outside)
				},
				.guides = {
					.color = Gfx::Color::Gray(0.93),
					.lineWidth = 1
				},
				.interlacing = {
					.color = Gfx::Color::Gray(0.97)
				}
			}
		},
		.legend = {
			{
				.paddingTop = Gfx::Length::Emphemeral(10.0/12.0),
				.paddingRight = Gfx::Length::Emphemeral(10.0/12.0),
				.paddingBottom = Gfx::Length::Emphemeral(10.0/12.0),
				.paddingLeft = Gfx::Length::Emphemeral(30.0/12.0)
			},
			{
				.backgroundColor = Gfx::Color(),
				.borderColor = Gfx::Color(),
				.borderWidth = 0,
			},
			.width = Gfx::Length::Emphemeral(150.0/12.0),
			.maxWidth = Gfx::Length::Relative(0.3),
			.title = {
				{
					.paddingTop = Gfx::Length::Emphemeral(5/14.0),
					.paddingRight = Gfx::Length::Emphemeral(5/14.0),
					.paddingBottom = Gfx::Length::Emphemeral(5/14.0),
					.paddingLeft = Gfx::Length::Emphemeral(5/14.0)
				},
				{
					.fontFamily = ::Anim::String(),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = Gfx::Length::Emphemeral(14.0/12.0)
				},
				{
					.color = Gfx::Color::Gray(0.6),
					.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::left),
					.backgroundColor = Gfx::Color(),
					.numberFormat = ::Text::NumberFormat::prefixed
				},
			},
			.label = {
				{
					.paddingTop = Gfx::Length::Emphemeral(5.0/12.0),
					.paddingRight = Gfx::Length::Emphemeral(5/12.0),
					.paddingBottom = Gfx::Length::Emphemeral(5/12.0),
					.paddingLeft = Gfx::Length::Emphemeral(5/12.0)
				},
				{
					.fontFamily = ::Anim::String(),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = Gfx::Length::Emphemeral(12.0/12.0)
				},
				{
					.color = Gfx::Color::Gray(0.6),
					.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::left),
					.backgroundColor = Gfx::Color(),
					.numberFormat = ::Text::NumberFormat::prefixed
				},
			},
			.marker = {
				.type = ::Anim::Interpolated<Legend::Marker::Type>
					(Legend::Marker::Type::circle),
				.size = Gfx::Length::Emphemeral(18.0/14.0)
			}
		},
		.title = {
			{
				.paddingTop = Gfx::Length::Emphemeral(25.0/26.0),
				.paddingRight = Gfx::Length::Emphemeral(10.0/26.0),
				.paddingBottom = Gfx::Length::Emphemeral(0),
				.paddingLeft = Gfx::Length::Emphemeral(10.0/26.0)
			},
			{
				.fontFamily = ::Anim::String(),
				.fontStyle = Gfx::Font::Style::normal,
				.fontWeight = Gfx::Font::Weight::Normal(),
				.fontSize = Gfx::Length::Emphemeral(26.0/12.0)
			},
			{
				.color = Gfx::Color::RGB(0x494949),
				.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::center),
				.backgroundColor = Gfx::Color(),
				.numberFormat = ::Text::NumberFormat::prefixed
			},
		},
		.tooltip = {
			.visible = 0
		},
		.data = {
			.colorGradient = Gfx::ColorGradient({
				{ 0.0, Gfx::Color::RGB(0x4171cd) },
				{ 0.25, Gfx::Color::RGB(0x8536c7) },
				{ 0.5, Gfx::Color::RGB(0xc33e7f) },
				{ 0.75, Gfx::Color::RGB(0xe9974e) },
				{ 1.0, Gfx::Color::RGB(0xf3f239) }
			}),
			.colorPalette = Draw::Palettes::Default(),
			.minLightness = -0.4,
			.maxLightness = 0.4,
			.lineWidth = 0.01,
			.lineMinWidth = 0.001,
			.lineMaxWidth = 0.1,
			.circleMinRadius = 2,
			.circleMaxRadius = 0.05,
			.barMaxPadding = 0.2,
			.barPaddingDecrease = 5.5,
			.columnMaxPadding = 0.2,
			.columnPaddingDecrease = 5.0
		}
	};
}
