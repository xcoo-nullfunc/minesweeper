extends TextureButton

'''
func _ready() -> void:
	self.renew_texture()
'''

func change_mode() -> void:
	$"../../../MST".touch_mode = $"../../../MST".touch_mode != true
	renew_texture()
	
func renew_texture() -> void:
	if($"../../../MST".touch_mode):
		self.texture_normal = $"..".hold_texture
	else:
		self.texture_normal = $"..".touch_texture
