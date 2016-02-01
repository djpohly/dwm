static void
bstack(Monitor *m)
{
	unsigned int i, n, w, mh, mx, tx;
	int x, y;
	Client *c, *held = NULL, **prevp;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		if (c->isheld && getrootptr(&x, &y)) {
			// Detach to reattach later
			held = c;
			detach(held);
		}
	if (n == 0)
		return;

	if (n > m->nmaster)
		mh = m->nmaster ? m->wh * m->mfact : 0;
	else
		mh = m->wh;
	prevp = &m->clients;
	for (i = mx = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
			if (held && m->wx + mx <= x && m->wy <= y && m->wx + mx + w > x && m->wy + mh > y) {
				resize(held, held->x, held->y, w - (2*held->bw), mh - (2*held->bw), 0);
				held->next = c;
				c = *prevp = held;
				held = NULL;
			} else
				resize(c, m->wx + mx, m->wy, w - (2*c->bw), mh - (2*c->bw), 0);
			mx += w;
		} else {
			w = (m->ww - tx) / (n - i);
			if (held && m->wx + tx <= x && m->wy + mh <= y && m->wx + tx + w > x && m->wy + m->wh > y) {
				resize(held, held->x, held->y, w - (2*held->bw), m->wh - mh - (2*held->bw), 0);
				held->next = c;
				c = *prevp = held;
				held = NULL;
			} else
				resize(c, m->wx + tx, m->wy + mh, w - (2*c->bw), m->wh - mh - (2*c->bw), 0);
			tx += w;
		}
		prevp = &c->next;
	}
	// If held window didn't get reattached anywhere, attach after last tiled
	if (held) {
		if (n <= m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - n + 1);
			resize(held, held->x, held->y, w - (2*held->bw), mh - (2*held->bw), 0);
		} else {
			w = (m->ww - tx);
			resize(held, held->x, held->y, w - (2*held->bw), m->wh - mh - (2*held->bw), 0);
		}
		held->next = *prevp;
		*prevp = held;
	}
}
