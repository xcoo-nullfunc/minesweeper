extends Label

var smile : String = "'v'"
var cool : String = "@v@"
var die : String = "X_X"

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


func set_face_smile(str_smile : String) -> void:
	self.smile = str_smile

func set_face_cool(str_cool : String) -> void:
	self.cool = str_cool

func set_face_die(str_die : String) -> void:
	self.dir = str_die

func face_smile() -> void:
	self.text = self.smile

func face_cool() -> void:
	self.text = self.cool

func face_die() -> void:
	self.text = self.die
