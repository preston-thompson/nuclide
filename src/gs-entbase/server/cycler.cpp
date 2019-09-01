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

// TODO: Complete this once damage is done

/*
 * cycler - Point Entity
The cycler entity is a model viewer of sorts. It places a model within the map which, when attacked, will cycle its animation.
Attributes
Name (targetname) - Name is a general attribute used to identify entities.
Pitch Yaw Roll (Y Z X) (angles) - Sets the pitch, yaw and roll angles of the model.
Animation Sequence (editor) (sequence) - Sets the animation the model should start in. Setting this to anything other than 0 seems to place the cycler into its triggered mode (see notes).
Model (model) - This defines which model file will be displayed by the entity. This accepts an input of the form "path/filename.mdl" (starting from the root folder). (Example: "models/scientist.mdl").
Render FX (renderfx) - Gives the model certain visual effects depending on Render Mode.
Render Mode (rendermode) - Allows special rendering modes to be applied to the given entity.
FX Amount (1 - 255) (renderamt) - In all but "Normal" Render Mode, alters how transparent the model is. Scales from 0 (invisible) to 255 (solid, normal).
FX Color (R G B) (rendercolour) - Seemingly unused.
Notes
	Triggering or pressing use on the cycler will freeze the model in its current animation. In this state, shooting the cycler will have it step through frames of the animation it was in. Triggering it or using it again will return it to its normal function.
	Given the way this entity behaves, there is little practical use for this entity beyond viewing animations (for which there are programs that are far more convenient).
*/

class cycler:CBaseTrigger
{
	void() cycler;
	virtual void() Respawn;
	virtual void(entity, int, int) vPain;
};

void cycler::vPain(entity attacker, int type, int damage)
{
	if (frame >= modelframecount(modelindex)) {
		frame = 0;
	} else {
		frame += 1;
	}
	health = 9999;
}

void cycler::Respawn(void)
{
	CBaseEntity::Respawn();
	solid = SOLID_BBOX;
	takedamage = DAMAGE_YES;
	health = 9999;
}

void cycler::cycler(void)
{
	CBaseEntity::CBaseEntity();
	Respawn();
}