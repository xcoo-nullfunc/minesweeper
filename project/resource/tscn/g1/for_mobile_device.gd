extends GridContainer

var touch_texture : Texture2D
var hold_texture : Texture2D

func _ready() -> void:
	if(OS.has_feature("mobile")):
		self.visible = true;
		self.touch_texture = preload("res://resource/image/mobile_buttons/open_mode.png")
		self.hold_texture = preload("res://resource/image/mobile_buttons/mark_mode.png")
	else:
		self.visible = false;
