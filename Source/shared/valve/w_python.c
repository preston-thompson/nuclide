/***
*
*	Copyright (c) 2016-2019 Marco 'eukara' Hladik. All rights reserved.
*
*	See the file LICENSE attached with the sources for usage details.
*
****/

.int python_cylinder;

enum
{
	PYTHON_IDLE1,
	PYTHON_FIDGET,
	PYTHON_FIRE1,
	PYTHON_RELOAD,
	PYTHON_HOLSTER,
	PYTHON_DRAW,
	PYTHON_IDLE2,
	PYTHON_IDLE3
};

void w_python_precache(void)
{
	precache_model("models/v_357.mdl");
	precache_model("models/w_357.mdl");
	precache_model("models/p_357.mdl");
	
	precache_sound("weapons/357_shot1.wav");
	precache_sound("weapons/357_shot2.wav");
}
void w_python_pickup(void)
{
	player pl = (player)self;
	pl.python_cylinder = 6;
}

string w_python_vmodel(void)
{
	return "models/v_357.mdl";
}
string w_python_wmodel(void)
{
	return "models/w_357.mdl";
}
string w_python_pmodel(void)
{
	return "models/p_357.mdl";
}
string w_python_deathmsg(void)
{
	return "";
}

void w_python_draw(void)
{
	Weapons_ViewAnimation(PYTHON_DRAW);
}

void w_python_holster(void)
{
	Weapons_ViewAnimation(PYTHON_HOLSTER);
}
void w_python_primary(void)
{
	player pl = (player)self;
	if (pl.w_attack_next > 0.0) {
		return;
	}

#ifdef SSQC
	TraceAttack_FireBullets(1, pl.origin + pl.view_ofs, 40);
#endif

	Weapons_ViewAnimation(PYTHON_FIRE1);
	Weapons_ViewPunchAngle([-10,0,0]);

	if (random() < 0.5) {
		Weapons_PlaySound(pl, CHAN_WEAPON, "weapons/357_shot1.wav", 1, ATTN_NORM);
	} else {
		Weapons_PlaySound(pl, CHAN_WEAPON, "weapons/357_shot2.wav", 1, ATTN_NORM);
	}
	
	pl.w_attack_next = 0.75f;
	pl.w_idle_next = 10.0f;
}
void w_python_secondary(void)
{
	player pl = (player)self;
	if (pl.w_attack_next > 0.0) {
		return;
	}
	/* Simple toggle of fovs */
	if (pl.viewzoom == 1.0f) {
		pl.viewzoom = 0.5;
	} else {
		pl.viewzoom = 1.0f;
	}
	pl.w_attack_next = 0.5f;
}
void w_python_reload(void)
{
	player pl = (player)self;
	if (pl.w_attack_next > 0.0) {
		return;
	}
	Weapons_ViewAnimation(PYTHON_RELOAD);
	pl.w_attack_next = 3.25f;
	pl.w_idle_next = 10.0f;
}
void w_python_release(void)
{
	player pl = (player)self;
	if (pl.w_idle_next > Math_Time()) {
		return;
	}

	int r = floor(random(0,3));

	switch (r) {
	case 0:
		Weapons_ViewAnimation(PYTHON_IDLE1);
		break;
	case 1:
		Weapons_ViewAnimation(PYTHON_IDLE2);
		break;
	case 2:
		Weapons_ViewAnimation(PYTHON_IDLE3);
		break;
	}

	pl.w_idle_next = Math_Time() + 15.0f;
}
void w_python_crosshair(void)
{
#ifdef CSQC
	static vector cross_pos;
	cross_pos = (video_res / 2) + [-12,-12];
	drawsubpic(cross_pos, [24,24], "sprites/crosshairs.spr_0.tga", [48/128,0], [0.1875, 0.1875], [1,1,1], 1, DRAWFLAG_NORMAL);
#endif
}

void w_python_hudpic(int s, vector pos)
{
#ifdef CSQC
	if (s) {
		drawsubpic(pos, [170,45], "sprites/640hud4.spr_0.tga", [0,90/256], [170/256,45/256], g_hud_color, 1, DRAWFLAG_ADDITIVE);
	} else {
		drawsubpic(pos, [170,45], "sprites/640hud1.spr_0.tga", [0,90/256], [170/256,45/256], g_hud_color, 1, DRAWFLAG_ADDITIVE);
	}
#endif
}

weapon_t w_python =
{
	ITEM_PYTHON,
	1,
	1,
	w_python_draw,
	w_python_holster,
	w_python_primary,
	w_python_secondary,
	w_python_reload,
	w_python_release,
	w_python_crosshair,
	w_python_precache,
	w_python_pickup,
	w_python_vmodel,
	w_python_wmodel,
	w_python_pmodel,
	w_python_deathmsg,
	w_python_hudpic
};

#ifdef SSQC
void weapon_357(void) {
	Weapons_InitItem(WEAPON_PYTHON);
}
void weapon_python(void) {
	Weapons_InitItem(WEAPON_PYTHON);
}
#endif