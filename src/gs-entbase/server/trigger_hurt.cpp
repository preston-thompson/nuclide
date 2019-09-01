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

#define SF_HURT_ONCE 			1 // Turn off once it fired the first time
#define SF_HURT_OFF 			2 // Needs to be triggered in order to work again
#define SF_HURT_NOPLAYERS 		8 // Don't hurt players
#define SF_HURT_FIREONPLAYER 	16 // Only call UseTarget functions when it's a player
#define SF_HURT_TOUCHPLAYER 	32 // Only hurt players

class trigger_hurt:CBaseTrigger
{
	float	m_flNextTrigger;
	int		m_iDamage;
	float	m_flDelay;
	void()	trigger_hurt;
	
	virtual void() Trigger;
	virtual void() Touch;
	virtual void() Respawn;
};

void trigger_hurt::Trigger(void)
{
	if (solid != SOLID_NOT) {
#ifdef GS_DEVELOPER
	print("trigger_hurt: de-activated.\n");
#endif
		solid = SOLID_NOT;
		touch = __NULL__;
	} else {
#ifdef GS_DEVELOPER
	print("trigger_hurt: activated.\n");
#endif
		solid = SOLID_TRIGGER;
		touch = Touch;
	}
}

void trigger_hurt::Touch(void)
{
	if (m_flNextTrigger > time) {
		return;
	} else if (other.takedamage == DAMAGE_NO) {
		return;
	} else if ((spawnflags & SF_HURT_TOUCHPLAYER) && !(other.flags & FL_CLIENT)) {
		return;
	} else if ((spawnflags & SF_HURT_NOPLAYERS) && (other.flags & FL_CLIENT)) {
		return;
	}

	if (spawnflags & SF_HURT_FIREONPLAYER) {
		if (other.flags & FL_CLIENT) {
			if (m_flDelay > 0) {
				CBaseTrigger::UseTargets_Delay(m_flDelay);
			} else {
				CBaseTrigger::UseTargets();
			}
		}
	} else {
		if (m_flDelay > 0) {
			CBaseTrigger::UseTargets_Delay(m_flDelay);
		} else {
			CBaseTrigger::UseTargets();
		}
	}

	Damage_Apply(other, this, m_iDamage, other.origin, FALSE);

	// Shut it down if used once
	if (spawnflags & SF_HURT_ONCE) {
		Trigger();
	}

	m_flNextTrigger = time + 0.5;
}

void trigger_hurt::Respawn(void)
{
#ifdef GS_DEVELOPER
	alpha = 0.5f;
#endif

	m_flNextTrigger = 0;

	if (spawnflags & SF_HURT_OFF) {
		solid = SOLID_NOT;
		touch = __NULL__;
	} else {
		solid = SOLID_TRIGGER;
		touch = Touch;
	}
}

void trigger_hurt::trigger_hurt(void)
{
	for (int i = 1; i < (tokenize(__fullspawndata) - 1); i += 2) {
		switch (argv(i)) {
		case "dmg":
			m_iDamage = stoi(argv(i+1));
			break;
		case "wait":
		case "delay":
			m_flDelay = stof(argv(i+1));
			break;
		default:
			break;
		}
	}

	CBaseEntity::CBaseEntity();
	CBaseTrigger::InitBrushTrigger();
	trigger_hurt::Respawn();
}