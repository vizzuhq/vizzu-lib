mergeInto(LibraryManager.library, {
	removeJsFunction: function(ptr) {
		Module.removeFunction(ptr);
	},
	jsconsolelog: function(str) {
		console.log(new Date().toISOString() + ': ' + UTF8ToString(str));
	},
	openUrl: function(url) {
		window.open(UTF8ToString(url), '_blank');
	},
	setCursor: function(cName) {
		Module.render.canvas().style.cursor = UTF8ToString(cName);
	},
	canvas_frameBegin: function() {
		Module.render.frameBegin();
	},
	canvas_frameEnd: function() {
		Module.render.frameEnd();
	},
	canvas_setClipRect: function(x, y, sizex, sizey) {
		var dc = Module.render.dc();
		dc.beginPath();
		dc.rect(x, y, sizex, sizey);
		dc.clip();
	},
	canvas_setClipCircle: function(x, y, radius) {
		var dc = Module.render.dc();
		dc.beginPath();
		dc.arc(x, y, radius, 0, 6.28318530718);
		dc.clip();
	},
	canvas_setClipPolygon: function() {
		var dc = Module.render.dc();
		dc.closePath();
		dc.clip();
		Module.render.endPolygonNotification();
	},
	canvas_setBrushColor: function(r, g, b, a) {
		var dc = Module.render.dc();
		dc.fillStyle = "rgba(" + r * 255 + "," + g * 255 + "," + b * 255 + "," + a + ")";
	},
	canvas_setLineColor: function(r, g, b, a) {
		var dc = Module.render.dc();
		dc.strokeStyle = "rgba(" + r * 255 + "," + g * 255  + "," + b * 255  + "," + a + ")";
	},
	canvas_setLineWidth: function(width) {
		var dc = Module.render.dc();
		dc.lineWidth = width;
		Module.render.lineWidthNotification(width);
	},
	canvas_setFont: function(font) {
		var dc = Module.render.dc();
		dc.font = UTF8ToString(font);
	},
	canvas_beginDropShadow: function() {
	},
	canvas_setDropShadowBlur: function(radius) {
		let dc = Module.render.dc();
		dc.shadowBlur = radius;
	},
	canvas_setDropShadowColor: function(r, g, b, a) {
		let dc = Module.render.dc();
		dc.shadowColor = "rgba(" + r * 255 + "," + g * 255 + "," + b * 255 + "," + a + ")";
	},
	canvas_setDropShadowOffset: function(x, y) {
		let dc = Module.render.dc();
		dc.shadowOffsetX = x;
		dc.shadowOffsetY = y;
	},
	canvas_endDropShadow: function() {
		let dc = Module.render.dc();
		dc.shadowBlur = 0;
		dc.shadowOffsetX = 0;
		dc.shadowOffsetY = 0;
		dc.shadowColor = "rgba(0, 0, 0, 0)";
	},
	canvas_beginPolygon: function() {
		var dc = Module.render.dc();
		dc.beginPath();
	},
	canvas_addPoint: function(x, y) {
		var dc = Module.render.dc();
		if (Module.render.startPolygonNotification())
			dc.moveTo(x, y);
		else
			dc.lineTo(x, y);
	},
	canvas_addBezier: function(c0x, c0y, c1x, c1y, x, y) {
		var dc = Module.render.dc();
		dc.bezierCurveTo(c0x, c0y, c1x, c1y, x, y);
	},
	canvas_endPolygon: function() {
		var dc = Module.render.dc();
		dc.closePath();
		dc.fill();
		if (Module.render.noneZeroLineWidth())
			dc.stroke();
		Module.render.endPolygonNotification();
	},
	canvas_rectangle: function(x, y, sizex, sizey) {
		var dc = Module.render.dc();
		dc.beginPath();
		dc.rect(x, y, sizex, sizey);
		dc.fill();
		if (Module.render.noneZeroLineWidth())
			dc.stroke();
	},
	canvas_circle: function(x, y, radius) {
		var dc = Module.render.dc();
		dc.beginPath();
		dc.arc(x, y, radius, 0, 6.28318530718);
		dc.fill();
		if (Module.render.noneZeroLineWidth())
			dc.stroke();
	},
	canvas_line: function(x1, y1, x2, y2) {
		var dc = Module.render.dc();
		dc.beginPath();
		dc.moveTo(x1, y1);
		dc.lineTo(x2, y2);
		if (Module.render.noneZeroLineWidth())
			dc.stroke();
	},
	canvas_textBoundary: function(text, sizeX, sizeY) {
		var dc = Module.render.dc();
		var metrics = dc.measureText(UTF8ToString(text));
		var width = metrics.width;
		metrics = dc.measureText('Op');
		var height = metrics.actualBoundingBoxAscent
		           + metrics.actualBoundingBoxDescent;
		setValue(sizeX, width, 'double');
		setValue(sizeY, height, 'double');
	},
	canvas_text: function(x, y, sizex, sizey, text) {
		var dc = Module.render.dc();
		dc.textAlign = "left";
		dc.textBaseline = "top";
		x = x + (sizex < 0 ? -sizex : 0);
		y = y + (sizey < 0 ? -sizey : 0);
		dc.fillText(UTF8ToString(text), x, y);
	},
	canvas_setBrushGradient: function(x1, y1, x2, y2, stopsCount, stops) {
		var dc = Module.render.dc();
		var grd = dc.createLinearGradient(x1, y1, x2, y2);
		for(var i = 0; i < stopsCount * 40; i += 40) {
			grd.addColorStop(
				getValue(stops + i, "double"), "rgba(" +
				getValue(stops + i + 8, "double") * 255 + "," +
				getValue(stops + i + 16, "double") * 255 + "," +
				getValue(stops + i + 24, "double") * 255 + "," +
				getValue(stops + i + 32, "double") + ")"
			);
		}
		dc.fillStyle = grd;
	},
	canvas_drawImage: function(imageId, x, y, sizex, sizey, alpha) {
	},
	canvas_transform: function(dx, dy, scale, rotate) {
		var dc = Module.render.dc();
		dc.translate(dx, dy);
		dc.rotate(rotate);
		dc.scale(scale, scale);
	},
	canvas_save: function() {
		var dc = Module.render.dc();
		dc.save();
	},
	canvas_restore: function() {
		var dc = Module.render.dc();
		dc.restore();
	},
	event_invoked: function(handlerId, param) {
		Module.events.invoke(handlerId, param);
	}
});
