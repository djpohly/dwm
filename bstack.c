static void
bstack(Monitor *m) {
	unsigned int i, n, w, mh, mx, tx;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mh = m->nmaster ? m->wh * m->mfact : 0;
	else
		mh = m->wh;
	for (i = mx = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->wy, w - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c);
		} else {
			w = (m->ww - tx) / (n - i);
			resize(c, m->wx + tx, m->wy + mh, w - (2*c->bw), m->wh - mh - (2*c->bw), 0);
			tx += WIDTH(c);
		}
}
