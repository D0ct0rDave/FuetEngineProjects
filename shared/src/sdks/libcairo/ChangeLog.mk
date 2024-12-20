# Creating ChangeLog files from git log:

# We always create a ChangeLog that contains the most recent changes, and
# multiple others for changes between major releases (other than the last such
# segment that we put in 'ChangeLog'.  The old ones are named
# ChangeLog.pre-X.Y where X.Y is the version number of the major release.

CURR_CHANGELOG_VERSION=$(CAIRO_VERSION_MAJOR).$$(echo "($(CAIRO_VERSION_MINOR)+1)/2*2" | bc)
# examines $version
PREV_CHANGELOG_VERSION=$$(if test "x$$(echo "($$version-0.1)*2/2"|bc)" = "x$$(echo "$$version*2/2"|bc)"; \
			  then echo "$$version-$$(echo "$$version" | sed 's/[0-9]/0/g;s/[0-9]$$/2/')"; \
			  else echo "$$version-1.0"; \
			  fi | bc | sed 's/[.]0*/./;s/^0[.]\?$$/initial/;s/[.]$$/.0/')

CHANGELOGS = ChangeLog \
	`version=$(CURR_CHANGELOG_VERSION); \
	version=$(PREV_CHANGELOG_VERSION); \
	while test "x$$version" != xinitial; do \
		echo ChangeLog.pre-$$version; \
		version=$(PREV_CHANGELOG_VERSION); \
	done`

MAINTAINERCLEANFILES += $(srcdir)/ChangeLog $(srcdir)/ChangeLog.pre-*
DISTCLEANFILES += $(srcdir)/ChangeLog.cache-*

changelogs:
	@$(MAKE) $(AM_MAKEFLAGS) $(CHANGELOGS)

dist-hook: changelogs
	changelogs="$(CHANGELOGS)"; \
	for changelog in $$changelogs; do \
		cp $(srcdir)/$$changelog $(distdir)/ 2>/dev/null || \
		cp $$changelog $(distdir)/; \
	done

$(srcdir)/ChangeLog:
	@if test -d "$(srcdir)/.git"; then \
		version=$(CURR_CHANGELOG_VERSION); \
		prev=$(PREV_CHANGELOG_VERSION).0; \
		nearest_tag=`git-describe | sed 's/-.*//'`; \
		before=$(srcdir)/ChangeLog.cache-$$prev..$$nearest_tag; \
		after=$(srcdir)/ChangeLog.cache-$$nearest_tag..; \
		$(MAKE) $(AM_MAKEFLAGS) $$before $$after && \
		echo Creating $@ && \
		{ cat $$after; echo; cat $$before; } > $@; \
	else \
		test -f $@ || \
		(echo A git checkout is required to generate $@ >&2 && \
		 echo A git checkout is required to generate this file >> $@); \
	fi

DISTCLEANFILES += ChangeLog.cache-*

ChangeLog.cache-*..: .git

ChangeLog%: $(srcdir)/ChangeLog%

$(srcdir)/ChangeLog.cache-% $(srcdir)/ChangeLog.pre-%:
	@echo Creating $@
	@if test -d "$(srcdir)/.git"; then \
	  (cd "$(srcdir)" && \
	  version=$$(echo "$@" | sed 's/.*ChangeLog\([.].*-\)\?//'); \
	  if echo "$@" | grep -q '^ChangeLog[.]cache'; then \
		spec=$$version; \
	  else \
		to=$$version; \
		test "x$$version" = x && version=$(CURR_CHANGELOG_VERSION); \
		from=$(PREV_CHANGELOG_VERSION); \
		test "x$$to" = x || version=$$version.0; \
		test "x$$from" = xinitial || from=$$from.0; \
		spec=$$from..$$to; \
	  fi; \
	  ./missing --run git-log --stat "$$spec") > $@.tmp \
	  && mv -f $@.tmp $@ \
	  || ($(RM) $@.tmp; \
	      echo Failed to generate $@, your $@ may be outdated >&2); \
	else \
	  echo A git checkout is required to generate $@ >&2; \
	fi

.PHONY: changelogs ChangeLog $(srcdir)/ChangeLog
