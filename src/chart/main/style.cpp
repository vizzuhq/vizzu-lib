#include "style.h"

#include "chart/rendering/palettes.h"

using namespace Vizzu;
using namespace Vizzu::Styles;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-designator"
#endif

Chart Chart::def()
{
	return {
		{
			.paddingTop = 0,
			.paddingRight = 0,
			.paddingBottom = 0,
			.paddingLeft = 0
		},
		{
			.backgroundColor = Gfx::Color::White(),
			.borderColor = Gfx::Color::White(),
			.borderWidth = 0,
		},
		{
			.fontFamily = ::Anim::String("Roboto"),
			.fontStyle = Gfx::Font::Style::normal,
			.fontWeight = Gfx::Font::Weight::Normal(),
			.fontSize = 11
		},
		.plot = {
			{
				.paddingTop = 50,
				.paddingRight = 50,
				.paddingBottom = 70,
				.paddingLeft = 70
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
				.renderingOrder = Marker::RenderingOrder::forward,
				.guides = {
					.color = Gfx::Color::Gray(0.93),
					.lineWidth = 1
				},
				.label = {
					{
						{
							.paddingTop = 5,
							.paddingRight = 5,
							.paddingBottom = 5,
							.paddingLeft = 5
						},
						{
							.fontFamily = ::Anim::String("Roboto"),
							.fontStyle = Gfx::Font::Style::normal,
							.fontWeight = Gfx::Font::Weight::Normal(),
							.fontSize = 11
						},
						{
							.color = Param<Gfx::Color>(),
							.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::center),
							.backgroundColor = Gfx::Color(),
							.overflow = Anim::Interpolated<Overflow>
								(Overflow::visible),
							.numberFormat = ::Text::NumberFormat::prefixed
						}
					},
					.position = Anim::Interpolated<MarkerLabel::Position>
						(MarkerLabel::Position::center),
					.filter = Gfx::ColorTransform::Lightness(0.8),
					.format = MarkerLabel::Format::valueFirst
				}
			},
			.axis = {
				.color = Gfx::Color::Gray(0.8),
				.title = {
					{
						.paddingTop = 22,
						.paddingRight = 0,
						.paddingBottom = 20,
						.paddingLeft = 0
					},
					{
						.fontFamily = ::Anim::String("Roboto"),
						.fontStyle = Gfx::Font::Style::normal,
						.fontWeight = Gfx::Font::Weight::Normal(),
						.fontSize = 14
					},
					{
						.color = Gfx::Color::Gray(0.6),
						.textAlign = Anim::Interpolated<Text::TextAlign>
							(Text::TextAlign::left),
						.backgroundColor = Gfx::Color(),
						.overflow = Anim::Interpolated<Overflow>
							(Overflow::visible),
						.numberFormat = ::Text::NumberFormat::prefixed
					},
				},
				.label = {
					{
						.paddingTop = 20,
						.paddingRight = 20,
						.paddingBottom = 25,
						.paddingLeft = 25
					},
					{
						.fontFamily = ::Anim::String("Roboto"),
						.fontStyle = Gfx::Font::Style::normal,
						.fontWeight = Gfx::Font::Weight::Normal(),
						.fontSize = 12
					},
					{
						.color = Gfx::Color("#abababFF"),
						.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::left),
						.backgroundColor = Gfx::Color(),
						.overflow = Anim::Interpolated<Overflow>
							(Overflow::hidden),
						.numberFormat = ::Text::NumberFormat::prefixed
					},
				},
				.ticks = {
					.color = Gfx::Color::Gray(0.8),
					.lineWidth = 1,
					.length = 5,
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
				.paddingTop = 10,
				.paddingRight = 10,
				.paddingBottom = 10,
				.paddingLeft = 30
			},
			{
				.backgroundColor = Gfx::Color(),
				.borderColor = Gfx::Color(),
				.borderWidth = 0,
			},
			.width = 100,
			.title = {
				{
					.paddingTop = 5,
					.paddingRight = 5,
					.paddingBottom = 5,
					.paddingLeft = 5
				},
				{
					.fontFamily = ::Anim::String("Roboto"),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = 14
				},
				{
					.color = Gfx::Color::Gray(0.77),
					.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::left),
					.backgroundColor = Gfx::Color(),
					.overflow = Anim::Interpolated<Overflow>(Overflow::visible),
					.numberFormat = ::Text::NumberFormat::prefixed
				},
			},
			.label = {
				{
					.paddingTop = 5,
					.paddingRight = 5,
					.paddingBottom = 5,
					.paddingLeft = 10
				},
				{
					.fontFamily = ::Anim::String("Roboto"),
					.fontStyle = Gfx::Font::Style::normal,
					.fontWeight = Gfx::Font::Weight::Normal(),
					.fontSize = 14
				},
				{
					.color = Gfx::Color::Gray(0.45),
					.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::left),
					.backgroundColor = Gfx::Color(),
					.overflow = Anim::Interpolated<Overflow>(Overflow::visible),
					.numberFormat = ::Text::NumberFormat::prefixed
				},
			},
			.marker = {
				.type = ::Anim::Interpolated<Legend::Marker::Type>
					(Legend::Marker::Type::circle),
				.size = 18
			}
		},
		.title = {
			{
				.paddingTop = 25,
				.paddingRight = 10,
				.paddingBottom = 0,
				.paddingLeft = 10
			},
			{
				.fontFamily = ::Anim::String("Roboto"),
				.fontStyle = Gfx::Font::Style::normal,
				.fontWeight = Gfx::Font::Weight::Normal(),
				.fontSize = 26
			},
			{
				.color = Gfx::Color("#494949FF"),
				.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::center),
				.backgroundColor = Gfx::Color(),
				.overflow = Anim::Interpolated<Overflow>(Overflow::visible),
				.numberFormat = ::Text::NumberFormat::prefixed
			},
		},
		.hint = {
			.visible = 0
		},
		.data = {
			.colorGradient = Gfx::ColorGradient::HeatMap5Color(),
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
