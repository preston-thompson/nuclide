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

/*QUAKED trigger_autosave (0 .5 .8) ?
"targetname"    Name

Volume that'll save the current game when a client enters its bounds.
This entity does not work in multiplayer.
*/

class trigger_autosave:CBaseTrigger
{
	float m_flDelay;
	void() trigger_autosave;
	virtual void() touch;
	virtual void() Respawn;
};

void trigger_autosave::touch(void)
{
	eActivator = other;

	/* saved text */
	WriteByte(MSG_MULTICAST, SVC_CGAMEPACKET);
	WriteByte(MSG_MULTICAST, EV_MESSAGE);
	WriteString(MSG_MULTICAST, "GAMESAVED");
	WriteString(MSG_MULTICAST, "common/menu2.wav");
	WriteFloat(MSG_MULTICAST, 1.0);
	WriteByte(MSG_MULTICAST, ATTN_NONE);
	msg_entity = this;
	multicast(origin, MULTICAST_ALL);

	localcmd("save autosave");
	Hide();

	if (m_flDelay > 0) {
		CBaseTrigger::UseTargets_Delay(m_flDelay);
	} else {
		CBaseTrigger::UseTargets();
	}
	solid = SOLID_NOT;
#ifdef GS_DEVELOPER
	alpha = 0.001f;
#endif
}

/* TODO: Make this redundant */
void trigger_autosave::Respawn(void)
{
	solid = SOLID_TRIGGER;
#ifdef GS_DEVELOPER
	alpha = 0.5f;
#endif
}

void trigger_autosave::trigger_autosave(void)
{	
	if (cvar("sv_playerslots") > 1) {
		remove(this);
		return;
	}
	for (int i = 1; i < (tokenize(__fullspawndata) - 1); i += 2) {
		switch (argv(i)) {
		case "delay":
			m_flDelay = stof(argv(i + 1));
			break;
		default:
			break;
		}
	}
	CBaseEntity::CBaseEntity();
	CBaseTrigger::InitBrushTrigger();
	trigger_autosave::Respawn();
}
