/*
 * Copyright (c) 2016-2019 Marco Hladik <marco@icculus.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

enum {
	USP45_IDLE,
	USP45_RELOAD,
	USP45_DRAW,
	USP45_SHOOT1,
	USP45_SHOOT2,
	USP45_SHOOT3
};

void
w_usp45_precache(void)
{
	precache_model("models/v_usp45.mdl");
	precache_model("models/w_usp45.mdl");
	precache_model("models/p_usp45.mdl");
	precache_sound("weapons/usp45-1.wav");
	precache_sound("weapons/usp45-2.wav");
}

void
w_usp45_updateammo(player pl)
{
#ifdef SSQC
	Weapons_UpdateAmmo(pl, pl.usp45_mag, pl.ammo_762mm, -1);
#endif
}

string
w_usp45_wmodel(void)
{
	return "models/w_usp45.mdl";
}

string
w_usp45_pmodel(void)
{
	return "models/p_usp45.mdl";
}

string
w_usp45_deathmsg(void)
{
	return "";
}

int
w_usp45_pickup(int new)
{
#ifdef SSQC
	player pl = (player)self;

	if (new) {
		pl.usp45_mag = 30;
	} else {
		if (pl.ammo_762mm < 90) {
			pl.ammo_762mm = bound(0, pl.ammo_762mm + 30, 90);
		} else {
			return FALSE;
		}
	}
#endif
	return TRUE;
}

void
w_usp45_draw(void)
{
#ifdef CSQC
	Weapons_SetModel("models/v_usp45.mdl");
	Weapons_ViewAnimation(USP45_DRAW);
#endif
}

void
w_usp45_primary(void)
{
	player pl = (player)self;

	if (pl.w_attack_next > 0.0) {
		return;
	}

#ifdef CSQC
	if (!pl.a_ammo1) {
		return;
	}

	View_SetMuzzleflash(MUZZLE_RIFLE);
	Weapons_ViewPunchAngle([-2,0,0]);

	int r = floor(random(0,3));
	switch (r) {
	case 0:
		Weapons_ViewAnimation(USP45_SHOOT1);
		break;
	case 1:
		Weapons_ViewAnimation(USP45_SHOOT2);
		break;
	default:
		Weapons_ViewAnimation(USP45_SHOOT3);
		break;
	}
#else
	if (!pl.usp45_mag) {
		return;
	}

	TraceAttack_FireBullets(1, pl.origin + pl.view_ofs, 8, [0.01,0,01], WEAPON_USP45);

	pl.usp45_mag--;

	if (self.flags & FL_CROUCHING)
		Animation_PlayerTopTemp(ANIM_SHOOT1HAND, 0.45f);
	else
		Animation_PlayerTopTemp(ANIM_CR_SHOOT1HAND, 0.45f);

	if (random() < 0.5) {
		sound(pl, CHAN_WEAPON, "weapons/usp45-1.wav", 1.0f, ATTN_NORM);
	} else {
		sound(pl, CHAN_WEAPON, "weapons/usp45-2.wav", 1.0f, ATTN_NORM);
	}
#endif

	pl.w_attack_next = 0.0955f;
}

void
w_usp45_reload(void)
{
	player pl = (player)self;

	if (pl.w_attack_next > 0.0) {
		return;
	}

#ifdef CSQC
	if (pl.a_ammo1 >= 30) {
		return;
	}
	if (!pl.a_ammo2) {
		return;
	}
#else
	if (pl.usp45_mag >= 30) {
		return;
	}
	if (!pl.ammo_762mm) {
		return;
	}

	Weapons_ReloadWeapon(pl, player::usp45_mag, player::ammo_762mm, 30);
	Weapons_UpdateAmmo(pl, pl.usp45_mag, pl.ammo_762mm, -1);
#endif

	Weapons_ViewAnimation(USP45_RELOAD);
	pl.w_attack_next = 2.0f;
}

float
w_usp45_aimanim(void)
{
	return self.flags & FL_CROUCHING ? ANIM_CR_AIM1HAND : ANIM_AIM1HAND;
}

void
w_usp45_hud(void)
{
#ifdef CSQC

	HUD_DrawAmmo1();
	HUD_DrawAmmo2();
	vector aicon_pos = video_mins + [video_res[0] - 48, video_res[1] - 42];
	drawsubpic(aicon_pos, [24,24], "sprites/640hud7.spr_0.tga", [0,72/128], [24/256, 24/128], g_hud_color, pSeat->ammo2_alpha, DRAWFLAG_ADDITIVE);
#endif
}

void
w_usp45_hudpic(int selected, vector pos, float a)
{
#ifdef CSQC
	if (selected) {
		drawsubpic(
			pos,
			[170,45],
			"sprites/640hud4.spr_0.tga",
			[0,90/256],
			[170/256,45/256],
			g_hud_color,
			1.0f,
			DRAWFLAG_ADDITIVE
		);
	} else {
		drawsubpic(
			pos,
			[170,45],
			"sprites/640hud1.spr_0.tga",
			[0,90/256],
			[170/256,45/256],
			g_hud_color,
			1.0f,
			DRAWFLAG_ADDITIVE
		);
	}
#endif
}

weapon_t w_usp45 =
{
	ITEM_USP45,
	1,
	0,
	"sprites/640hud1.spr_0.tga",
	[32,16],
	[192,16],
	w_usp45_draw,
	__NULL__,
	w_usp45_primary,
	__NULL__,
	w_usp45_reload,
	__NULL__,
	w_usp45_hud,
	w_usp45_precache,
	w_usp45_pickup,
	w_usp45_updateammo,
	w_usp45_wmodel,
	w_usp45_pmodel,
	w_usp45_deathmsg,
	w_usp45_aimanim,
	w_usp45_hudpic
};

#ifdef SSQC
void
weapon_usp45(void)
{
	Weapons_InitItem(WEAPON_USP45);
}
#endif