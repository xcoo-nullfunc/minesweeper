extends ColorRect

func _input(event: InputEvent) -> void:
	if(event is InputEventKey):
		if(event.keycode == KEY_O and event.pressed and not event.echo):
			if(self.visible == false):
				self.visible = true
			else:
				self.visible = false
				$"../..".save_options()

func change_visibal() -> void:
	self.visible = !self.visible
