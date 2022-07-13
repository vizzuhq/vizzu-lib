export default class Presets {
  constructor(chart) {
    this._presetConfigs = {
      column: {
        channels: {
          x: "x",
          y: "y",
          label: "y",
        },
      },
      groupedColumn: {
        channels: {
          x: ["groupedBy", "x"],
          y: "y",
          label: "y",
          color: "x",
        },
      },
      stackedColumn: {
        channels: {
          x: "x",
          y: ["y", "stackedBy"],
          label: "y",
          color: "stackedBy",
        },
      },
      splittedColumn: {
        channels: {
          x: "x",
          y: ["y", "splittedBy"],
          color: "splittedBy",
        },
        split: true,
      },
      percentageColumn: {
        channels: {
          x: "x",
          y: ["y", "stackedBy"],
          color: "stackedBy",
        },
        align: "stretch",
      },
      waterfall: {
        channels: {
          x: "x",
          y: ["y", "x"],
          label: "y",
          color: "y",
        },
        align: "stretch",
      },
      mekko: {
        channels: {
          x: ["x", "groupedBy"],
          y: ["y", "stackedBy"],
          color: "stackedBy",
          label: "groupedBy",
        },
      },
      marimekko: {
        channels: {
          x: ["x", "groupedBy"],
          y: ["y", "stackedBy"],
          color: "stackedBy",
          label: "groupedBy",
        },
        align: "stretch",
        orientation: "horizontal",
      },
      bar: {
        channels: {
          x: "x",
          y: "y",
          label: "x",
        },
      },
      groupedBar: {
        channels: {
          x: "x",
          y: ["groupedBy", "y"],
          label: "x",
          color: "y",
        },
      },
      stackedBar: {
        channels: {
          x: ["x", "stackedBy"],
          y: "y",
          label: "x",
          color: "stackedBy",
        },
      },
      splittedBar: {
        channels: {
          x: ["x", "splittedBy"],
          y: "y",
          color: "splittedBy",
        },
        split: true,
      },
      percentageBar: {
        channels: {
          x: ["x", "stackedBy"],
          y: "y",
          color: "stackedBy",
        },
        align: "stretch",
      },
      lollipop: {
        channels: {
          x: "x",
          y: "y",
        },
        geometry: "circle",
      },
      scatter: {
        channels: {
          x: "x",
          y: "y",
          noop: "dividedBy",
        },
        geometry: "circle",
      },
      bubbleplot: {
        channels: {
          x: "x",
          y: "y",
          color: "color",
          size: "size",
          noop: "dividedBy"
        },
        geometry: "circle",
      },
      area: {
        channels: {
          x: "x",
          y: "y",
        },
        geometry: "area",
      },
      stackedArea: {
        channels: {
          x: "x",
          y: ["y", "stackedBy"],
          color: "stackedBy",
        },
        geometry: "area",
      },
      percentageArea: {
        channels: {
          x: "x",
          y: ["y", "stackedBy"],
          color: "stackedBy",
        },
        align: "stretch",
        geometry: "area",
      },
      splittedArea: {
        channels: {
          x: "x",
          y: ["y", "splittedBy"],
          color: "splittedBy",
        },
        split: true,
        geometry: "area",
      },
      stream: {
        channels: {
          x: "x",
          y: ["y", "stackedBy"],
          color: "stackedBy",
        },
        geometry: "area",
        align: "center",
      },
      verticalStream: {
        channels: {
          x: ["x", "stackedBy"],
          y: "y",
          color: "stackedBy",
        },
        geometry: "area",
        align: "center",
      },
      violin: {
        channels: {
          x: "x",
          y: ["y", "splittedBy"],
          color: "splittedBy",
        },
        geometry: "area",
        align: "center",
        split: true,
      },
      verticalViolin: {
        channels: {
          x: ["x", "splittedBy"],
          y: "y",
          color: "splittedBy",
        },
        geometry: "area",
        align: "center",
        split: true,
      },
      line: {
        channels: {
          x: "x",
          y: "y",
          color: "dividedBy",
        },
        geometry: "line",
      },
      verticalLine: {
        channels: {
          x: "x",
          y: "y",
          color: "dividedBy",
        },
        geometry: "line",
      },
      pie: {
        channels: {
          x: ["angle", "by"],
          color: "by",
          label: "angle"
        },
        coordSystem: "polar",
      },
      polarColumn: {
        channels: {
          x: "angle",
          y: "radius",
        },
        coordSystem: "polar",
      },
      polarStackedColumn: {
        channels: {
          x: "angle",
          y: ["radius", "stackedBy"],
          color: "stackedBy",
        },
        coordSystem: "polar",
      },
      variableRadiusPie: {
        channels: {
          x: ["angle", "by"],
          y: "radius",
          color: "by",
        },
        coordSystem: "polar",
      },
      radialBar: {
        channels: {
          x: "angle",
          y: "radius",
        },
        coordSystem: "polar",
      },
      radialStackedBar: {
        channels: {
          x: ["angle", "stackedBy"],
          y: "radius",
          color: "stackedBy",
        },
        coordSystem: "polar",
      },
      donut: {
        channels: {
          x: ["angle", "stackedBy"],
          color: "stackedBy",
        },
        coordSystem: "polar",
      },
      nestedDonut: {
        channels: {
          x: ["angle", "stackedBy"],
          y: "radius",
          color: "stackedBy",
          label: "angle",
        },
        coordSystem: "polar",
        align: "stretch",
      },
      polarScatter: {
        channels: {
          x: "angle",
          y: "radius",
          noop: "dividedBy",
        },
        coordSystem: "polar",
        geometry: "circle",
      },
      polarLine: {
        channels: {
          x: "angle",
          y: "radius",
          color: "dividedBy",
        },
        coordSystem: "polar",
        geometry: "line",
      },
      treemap: {
        channels: {
          size: ["size", "color"],
          color: "color",
          label: "color",
        },
      },
      stackedTreemap: {
        channels: {
          size: ["size", "dividedBy"],
          color: "color",
          label: "dividedBy",
          lightness: "size"
        },
      },
      heatmap: {
        channels: {
          x: "x",
          y: "y",
          lightness: "lightness",
        },
      },
      bubble: {
        channels: {
          size: "size",
          color: "color",
          label: "color",
        },
        geometry: "circle",
      },
      stackedBubble: {
        channels: {
          size: ["size", "stackedBy"],
          color: "color",
        },
        geometry: "circle",
      },
    };

    for (let key in this._presetConfigs) {
      this[key] = (config) => {
        return this._buildPresetConfig(key, config);
      };
    }
  }

  _nullConfig() {
    return {
      align: "none",
      coordSystem: "cartesian",
      orientation: "horizontal",
      rotate: 0,
      split: false,
      geometry: "rectangle",
      channels: {
        x: null,
        y: null,
        color: null,
        lightness: null,
        size: null,
        noop: null,
        label: null,
      },
    };
  }

  _createPresetConfig(presetName) {
    let presetConfig = this._presetConfigs[presetName];
    let nullConfig = this._nullConfig();
    let channelBase = Object.assign(nullConfig.channels, presetConfig.channels);
    let base = Object.assign(this._nullConfig(), presetConfig);
    base.channels = channelBase;
    return base;
  }

  _getChannelCopy(channel) {
    if (channel === null) return null;
    if (channel === undefined) return null;
    if (Array.isArray(channel)) channel.map((v) => v);
    return [channel];
  }

  _fillChannels(presetConfig, config) {
    if (!config) return;
    let channels = presetConfig.channels;
    for (let channel in channels) {
      if (typeof channels[channel] === "string") {
        channels[channel] = this._getChannelCopy(config[channels[channel]]);
      } else if (Array.isArray(channels[channel])) {
        let newChannel = [];
        for (let i = 0; i < channels[channel].length; i++) {
          let channelConfig = this._getChannelCopy(config[channels[channel][i]]);
          if (channelConfig !== null) {
            newChannel.push(channelConfig);
          }
        }
        channels[channel] = newChannel.length > 0 ? newChannel.flat() : null;
      }
    }
  }

  _setupUserParams(base, config) {
    ["legend", "title", "reverse", "sort"].forEach((key) => {
      if (config[key] !== undefined) {
        base[key] = config[key];
      }
    });
  }

  _buildPresetConfig(presetName, config) {
    let presetConfig = this._createPresetConfig(presetName);
    this._fillChannels(presetConfig, config);
    this._setupUserParams(presetConfig, config);
    return presetConfig;
  }
}
