export default class Events {
  constructor(vizzu) {
    this.vizzu = vizzu;
    this.module = vizzu.module;
    this.eventHandlers = new Map();
  }

  add(eventName, handler) {
    if (eventName !== "" + eventName) {
      throw new Error("first parameter should be string");
    }

    let cname = this.vizzu._toCString(eventName);
    try {
      let id = this.vizzu._call(this.module._addEventListener)(cname);
      this.eventHandlers.set(id, handler);
    } finally {
      this.module._free(cname);
    }
  }

  remove(eventName, handler) {
    let cname = this.vizzu._toCString(eventName);
    if (eventName !== "" + eventName) {
      throw new Error("first parameter should be string");
    }
    try {
      if (!handler) {
        this.vizzu._call(this.module._removeEventListener)(cname, 0);
      } else {
        let handlerIdx = null;

        this.eventHandlers.forEach((h, idx) => {
          if (h === handler) {
            handlerIdx = idx;
          }
        });

        if (handlerIdx !== null) {
          // handler found
          this.vizzu._call(this.module._removeEventListener)(cname, handlerIdx);
        } else {
          throw new Error("unknown event handler");
        }
      }
    } finally {
      this.eventHandlers.delete(handler);
      this.module._free(cname);
    }
  }

  invoke(handlerId, param) {
    try {
      if (this.eventHandlers.has(handlerId)) {
        let eventParam = JSON.parse(this.vizzu._fromCString(param));
        eventParam.preventDefault = () => {
          this.vizzu._call(this.module._event_preventDefault)();
        };
        if (eventParam.data && eventParam.data.markerId) {
          eventParam.data.getMarker = this._getMarkerProxy(
            eventParam.data.markerId
          );
        }
        if (
          eventParam.event.endsWith("-draw") ||
          eventParam.event.startsWith("draw-")
        ) {
          eventParam.renderingContext = this.vizzu.render.dc();
        }
        this.eventHandlers.get(handlerId)(eventParam);
      }
    } catch (e) {
      console.log("exception in event handler: " + e);
    }
  }

  _getMarkerProxy(markerId) {
    let markerData = null;
    return () => {
      if (!markerData) {
        let cStr = this.vizzu._call(this.vizzu.module._chart_markerData)(
          markerId
        );
        markerData = JSON.parse(this.vizzu.module.UTF8ToString(cStr));
      }
      return markerData;
    };
  }
}
