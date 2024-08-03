#include "events.h"

#include "base/util/eventdispatcher.h"

namespace Vizzu
{

Events::Events(Util::EventDispatcher &ed) :
    draw{
        .begin = ed.createEvent("draw-begin"),
        .background = ed.createEvent("background-draw"),
        .title = ed.createEvent("title-draw"),
        .subtitle = ed.createEvent("subtitle-draw"),
        .caption = ed.createEvent("caption-draw"),
        .logo = ed.createEvent("logo-draw"),
        .legend{.background =
                    ed.createEvent("legend-background-draw"),
            .title = ed.createEvent("legend-title-draw"),
            .label = ed.createEvent("legend-label-draw"),
            .marker = ed.createEvent("legend-marker-draw"),
            .bar = ed.createEvent("legend-bar-draw")},
        .plot{.background = ed.createEvent("plot-background-draw"),
            .area = ed.createEvent("plot-area-draw"),
            .marker{.base = ed.createEvent("plot-marker-draw"),
                .label = ed.createEvent("plot-marker-label-draw"),
                .guide = ed.createEvent("plot-marker-guide-draw")},
            .axis{.base = ed.createEvent("plot-axis-draw"),
                .title = ed.createEvent("plot-axis-title-draw"),
                .label = ed.createEvent("plot-axis-label-draw"),
                .tick = ed.createEvent("plot-axis-tick-draw"),
                .guide = ed.createEvent("plot-axis-guide-draw"),
                .interlacing =
                    ed.createEvent("plot-axis-interlacing-draw")}},
        .complete = ed.createEvent("draw-complete"),
    },
    animation{.begin = ed.createEvent("animation-begin"),
        .update = ed.createEvent("update"),
        .complete = ed.createEvent("animation-complete")}
{}

}