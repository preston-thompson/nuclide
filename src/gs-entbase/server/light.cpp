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

class light:CBaseTrigger
{
	int m_iEnabled;
	float m_flStyle;
	string m_strPattern;

	void() light;
	virtual void() Trigger;
	virtual void() Respawn;
};

void light::Trigger(void)
{
	if (m_iEnabled == TRUE) {
		lightstyle(m_flStyle, "a");
		m_iEnabled = FALSE;
	} else {
		lightstyle(m_flStyle, m_strPattern);
		m_iEnabled = TRUE;
	}
}

void light::Respawn(void)
{
	if (spawnflags & 1) {
		lightstyle(m_flStyle, "a");
		m_iEnabled = FALSE;
	} else {
		lightstyle(m_flStyle, m_strPattern);
		m_iEnabled = TRUE;
	}
}

void light::light(void)
{
	for (int i = 1; i < (tokenize(__fullspawndata) - 1); i += 2) {
		switch (argv(i)) {
		case "pattern":
			m_strPattern = argv(i+1);
			break;
		case "style":
			m_flStyle = stof(argv(i+1));
			style = __NULL__;
			break;
		default:
			break;
		}
	}

	if (!m_strPattern) {
		m_strPattern = "m";
	}

	light::Respawn();
}

CLASSEXPORT(light_spot, light)
CLASSEXPORT(light_environment, light)
