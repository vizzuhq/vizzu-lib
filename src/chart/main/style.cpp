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
			.fontSize = 11
		},
		.plot = {
			{
				.paddingTop = Gfx::Length::Absolute(50),
				.paddingRight = Gfx::Length::Absolute(50),
				.paddingBottom = Gfx::Length::Absolute(70),
				.paddingLeft = Gfx::Length::Absolute(70)
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
					.color = Gfx::Color::Gray(0.93),
					.lineWidth = 1
				},
				.label = {
					{
						{
							.paddingTop = Gfx::Length::Absolute(5),
							.paddingRight = Gfx::Length::Absolute(5),
							.paddingBottom = Gfx::Length::Absolute(5),
							.paddingLeft = Gfx::Length::Absolute(5)
						},
						{
							.fontFamily = ::Anim::String("Roboto, sans-serif"),
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
						.paddingTop = Gfx::Length::Absolute(22),
						.paddingRight = Gfx::Length::Absolute(0),
						.paddingBottom = Gfx::Length::Absolute(20),
						.paddingLeft = Gfx::Length::Absolute(0)
					},
					{
						.fontFamily = ::Anim::String("Roboto, sans-serif"),
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
						.paddingTop = Gfx::Length::Absolute(20),
						.paddingRight = Gfx::Length::Absolute(20),
						.paddingBottom = Gfx::Length::Absolute(25),
						.paddingLeft = Gfx::Length::Absolute(25)
					},
					{
						.fontFamily = ::Anim::String("Roboto, sans-serif"),
						.fontStyle = Gfx::Font::Style::normal,
						.fontWeight = Gfx::Font::Weight::Normal(),
						.fontSize = 12
					},
					{
						.color = Gfx::Color::RGB(0xababab),
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
				.paddingTop = Gfx::Length::Absolute(10),
				.paddingRight = Gfx::Length::Absolute(10),
				.paddingBottom = Gfx::Length::Absolute(10),
				.paddingLeft = Gfx::Length::Absolute(30)
			},
			{
				.backgroundColor = Gfx::Color(),
				.borderColor = Gfx::Color(),
				.borderWidth = 0,
			},
			.width = 100,
			.title = {
				{
					.paddingTop = Gfx::Length::Absolute(5),
					.paddingRight = Gfx::Length::Absolute(5),
					.paddingBottom = Gfx::Length::Absolute(5),
					.paddingLeft = Gfx::Length::Absolute(5)
				},
				{
					.fontFamily = ::Anim::String("Roboto, sans-serif"),
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
					.paddingTop = Gfx::Length::Absolute(5),
					.paddingRight = Gfx::Length::Absolute(5),
					.paddingBottom = Gfx::Length::Absolute(5),
					.paddingLeft = Gfx::Length::Absolute(10)
				},
				{
					.fontFamily = ::Anim::String("Roboto, sans-serif"),
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
				.paddingTop = Gfx::Length::Absolute(25),
				.paddingRight = Gfx::Length::Absolute(10),
				.paddingBottom = Gfx::Length::Absolute(0),
				.paddingLeft = Gfx::Length::Absolute(10)
			},
			{
				.fontFamily = ::Anim::String("Roboto, sans-serif"),
				.fontStyle = Gfx::Font::Style::normal,
				.fontWeight = Gfx::Font::Weight::Normal(),
				.fontSize = 26
			},
			{
				.color = Gfx::Color::RGB(0x494949),
				.textAlign = Anim::Interpolated<Text::TextAlign>
								(Text::TextAlign::center),
				.backgroundColor = Gfx::Color(),
				.overflow = Anim::Interpolated<Overflow>(Overflow::visible),
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
