(() => {
  const query = (selector, root = document) => root.querySelector(selector);
  const loginForm = query("#login-form");
  const signupForm = query("#signup-form");
  const authScreen = query(".auth-screen");
  const authMessage = query("#auth-message");
  const guestLinks = [...document.querySelectorAll('.auth-options a[href="#home"], .guest-link')];
  const accountsKey = "moodify-accounts";
  const sessionKey = "moodify-session";
  const iterations = 310000;
  const encoder = new TextEncoder();

  if (!loginForm || !signupForm || !authScreen || !authMessage) {
    throw new Error("Moodify sign-in couldn't start because its login form is missing.");
  }

  const setMessage = (message = "", success = false) => {
    authMessage.textContent = message;
    authMessage.hidden = !message;
    authMessage.classList.toggle("is-success", success);
    authMessage.classList.toggle("is-error", Boolean(message) && !success);
  };

  const hex = (buffer) =>
    [...new Uint8Array(buffer)]
      .map((byte) => byte.toString(16).padStart(2, "0"))
      .join("");

  const saltFromHex = (value) => {
    if (typeof value !== "string" || !/^[0-9a-f]{32}$/i.test(value)) {
      throw new Error("A saved account has invalid password data. Please create the account again.");
    }

    return new Uint8Array(value.match(/.{2}/g).map((part) => parseInt(part, 16)));
  };

  const hashPassword = async (password, salt) => {
    if (!window.crypto?.subtle || !window.crypto?.getRandomValues) {
      throw new Error("Secure password hashing isn't available. Open Moodify in an up-to-date browser.");
    }

    const key = await crypto.subtle.importKey("raw", encoder.encode(password), "PBKDF2", false, ["deriveBits"]);
    return hex(
      await crypto.subtle.deriveBits(
        { name: "PBKDF2", salt, iterations, hash: "SHA-256" },
        key,
        256,
      ),
    );
  };

  const readAccounts = () => {
    let data;
    try {
      data = localStorage.getItem(accountsKey);
    } catch (error) {
      throw new Error(`Account storage is unavailable in this browser: ${error.message}`);
    }

    if (!data) return [];

    let accounts;
    try {
      accounts = JSON.parse(data);
    } catch {
      throw new Error("Saved account data could not be read. Clear this browser's Moodify data and try again.");
    }
    if (!Array.isArray(accounts)) {
      throw new Error("Saved account data has an invalid format.");
    }
    return accounts;
  };

  const writeStorage = (key, value) => {
    try {
      localStorage.setItem(key, JSON.stringify(value));
    } catch (error) {
      throw new Error(`Moodify couldn't save data in this browser: ${error.message}`);
    }
  };

  const constantTimeEqual = (first, second) => {
    if (first.length !== second.length) return false;
    let difference = 0;
    for (let index = 0; index < first.length; index += 1) {
      difference |= first.charCodeAt(index) ^ second.charCodeAt(index);
    }
    return difference === 0;
  };

  const moodLabels = {
    Happy: "😊 Happy",
    Sad: "😢 Sad",
    Romantic: "💗 Romantic",
    Chill: "😌 Chill",
    Focused: "🎯 Focused",
    Melancholy: "🌧️ Melancholy",
    Energetic: "⚡ Energetic",
  };
  const formatMood = (mood) => mood.split(", ").map((item) => moodLabels[item] || item).join(", ");

  const parseCsv = (text) => {
    const rows = [];
    let row = [];
    let field = "";
    let quoted = false;

    for (let index = 0; index < text.length; index += 1) {
      const character = text[index];
      if (character === '"') {
        if (quoted && text[index + 1] === '"') {
          field += '"';
          index += 1;
        } else {
          quoted = !quoted;
        }
      } else if (character === "," && !quoted) {
        row.push(field);
        field = "";
      } else if ((character === "\n" || character === "\r") && !quoted) {
        if (character === "\r" && text[index + 1] === "\n") index += 1;
        row.push(field);
        if (row.some((value) => value !== "")) rows.push(row);
        row = [];
        field = "";
      } else {
        field += character;
      }
    }

    if (field || row.length) {
      row.push(field);
      rows.push(row);
    }
    return rows;
  };

  const loadMusicTracks = async () => {
    const response = await fetch("songs.csv", { cache: "no-store" });
    if (!response.ok) throw new Error(`Song catalog request failed (${response.status}).`);

    const rows = parseCsv(await response.text());
    if (rows.length < 2) throw new Error("The shared song catalog is empty.");

    return rows.slice(1).map((fields) => {
      const [numericId, title, artist, mood, album, genre, duration, art, webId] = fields;
      const slug = (value) => value.toLocaleLowerCase().replace(/[^a-z0-9]+/g, "-").replace(/^-|-$/g, "");
      return {
        id: webId || `${slug(title)}-${slug(artist)}`,
        numericId: Number(numericId),
        title,
        artist,
        mood,
        moodDisplay: formatMood(mood),
        album,
        genre,
        duration,
        art,
      };
    });
  };

  let musicTracks = [];
  const trackMoods = (track) => track.mood.split(", ");
  const normalizeSearchText = (value) =>
    String(value)
      .toLocaleLowerCase()
      .replace(/[\p{Extended_Pictographic}\p{Emoji_Presentation}\p{Emoji_Component}]/gu, " ")
      .replace(/[^\p{L}\p{N}]+/gu, " ")
      .trim();

  const trackMatchesSearch = (track, search) => {
    if (!search) return true;

    const searchableText = normalizeSearchText([
      track.title,
      track.artist,
      track.album,
      track.mood,
      track.genre,
    ].join(" "));

    return normalizeSearchText(search)
      .split(/\s+/)
      .every((term) => searchableText.includes(term));
  };
  const catalogRows = query("#catalog-rows");
  const catalogCount = query("#catalog-count");
  const catalogMessage = query("#catalog-message");
  const searchInput = query("#music-search");
  const moodOptions = query("#mood-options");
  const currentMoodLabel = query("#mood-current");
  const discoveryMood = query("#discovery-mood");
  const discoveryTrackTitle = query("#discovery-track-title");
  const discoveryTrackArtist = query("#discovery-track-artist");
  const surpriseTrackButton = query("#surprise-track");
  const displayDiscoveryButton = query("#display-discovery");
  const likedFilter = query("#liked-only");
  const likedCount = query("#liked-count");
  const playerLike = query("#player-like");
  const playCount = query("#tracks-played-count");
  const topArtistName = query("#top-artist-name");
  const topArtistSummary = query("#top-artist-summary");
  const topArtistPlayCount = query("#artist-play-count");
  const topArtistGenres = query("#top-artist-genres");
  const genreRecommendations = query("#genre-recommendations");
  let likes = new Set();
  let likesStorageKey = "moodify-liked-songs:guest";
  let displayCounts = {};
  let displayCountsStorageKey = "moodify-display-counts:guest";
  let selectedTrackId = null;
  let likedOnly = false;
  let searchTerm = "";
  let selectedMood = "";
  let discoveryTrack = null;

  if (!catalogRows || !catalogCount || !catalogMessage || !searchInput || !moodOptions || !currentMoodLabel
    || !discoveryMood || !discoveryTrackTitle || !discoveryTrackArtist || !surpriseTrackButton || !displayDiscoveryButton
    || !likedFilter || !likedCount || !playerLike
    || !playCount || !topArtistName || !topArtistSummary || !topArtistPlayCount || !topArtistGenres || !genreRecommendations) {
    throw new Error("Moodify's sample playlist couldn't start because part of its interface is missing.");
  }

  const likeKeyForUser = (user) =>
    user.type === "account" ? `moodify-liked-songs:${user.accountId}` : "moodify-liked-songs:guest";

  const displayCountsKeyForUser = (user) =>
    user.type === "account" ? `moodify-display-counts:${user.accountId}` : "moodify-display-counts:guest";

  const updateLikeButton = (button, track) => {
    const liked = likes.has(track.id);
    button.textContent = liked ? "♥" : "♡";
    button.setAttribute("aria-pressed", String(liked));
    button.setAttribute("aria-label", `${liked ? "Unlike" : "Like"} ${track.title}`);
  };

  const updatePlayerLike = () => {
    const track = musicTracks.find((item) => item.id === selectedTrackId);
    playerLike.disabled = !track;
    if (track) {
      updateLikeButton(playerLike, track);
    } else {
      playerLike.textContent = "♡";
      playerLike.setAttribute("aria-label", "Like current song");
      playerLike.setAttribute("aria-pressed", "false");
    }
  };

  const topArtist = () => {
    const artists = new Map();
    musicTracks.forEach((track) => {
      const plays = displayCounts[track.id] || 0;
      const liked = likes.has(track.id);
      if (!plays && !liked) return;
      const artist = artists.get(track.artist) || { name: track.artist, plays: 0, likedTracks: 0, tracks: [] };
      artist.plays += plays;
      if (liked) artist.likedTracks += 1;
      artist.tracks.push(track);
      artists.set(track.artist, artist);
    });

    return [...artists.values()].sort(
      (first, second) => second.likedTracks - first.likedTracks
        || second.plays - first.plays
        || first.name.localeCompare(second.name),
    )[0] || null;
  };

  const renderArtistRecommendations = () => {
    const favorite = topArtist();
    topArtistGenres.replaceChildren();
    genreRecommendations.replaceChildren();

    if (!favorite) {
      topArtistName.textContent = "Like or display a song to find your top artist";
      topArtistSummary.textContent = "Your top artist is based on liked songs first, then track selections.";
      topArtistPlayCount.textContent = "YOUR TOP ARTIST";
    } else {
      topArtistName.textContent = favorite.name;
      topArtistSummary.textContent = `${favorite.likedTracks} liked ${favorite.likedTracks === 1 ? "song" : "songs"} · ${favorite.plays} ${favorite.plays === 1 ? "track selection" : "track selections"}.`;
      topArtistPlayCount.textContent = `${favorite.likedTracks} ${favorite.likedTracks === 1 ? "LIKE" : "LIKES"} · ${favorite.plays} ${favorite.plays === 1 ? "DISPLAY" : "DISPLAYS"} · TOP ARTIST`;

      const artistGenres = [...new Set(favorite.tracks.map((track) => track.genre))].sort();
      artistGenres.forEach((genre) => {
        const chip = document.createElement("span");
        chip.className = "genre-chip";
        chip.textContent = genre;
        topArtistGenres.append(chip);
      });
    }

    const genres = [...new Set(musicTracks.map((track) => track.genre))].sort();
    genres.forEach((genre) => {
      const section = document.createElement("section");
      section.className = "genre-recommendation";
      const heading = document.createElement("h4");
      heading.textContent = genre;
      const list = document.createElement("div");
      list.className = "genre-track-list";

      const tracks = musicTracks
        .filter((track) => track.genre === genre)
        .sort((first, second) => {
          const firstIsFavorite = favorite && first.artist === favorite.name ? 1 : 0;
          const secondIsFavorite = favorite && second.artist === favorite.name ? 1 : 0;
          return secondIsFavorite - firstIsFavorite
            || Number(likes.has(second.id)) - Number(likes.has(first.id))
            || (displayCounts[second.id] || 0) - (displayCounts[first.id] || 0)
            || first.title.localeCompare(second.title);
        });

      tracks.forEach((track) => {
        const suggestion = document.createElement("button");
        suggestion.className = "genre-track";
        suggestion.type = "button";
        suggestion.textContent = `${track.title} · ${track.artist}`;
        if (favorite && track.artist === favorite.name) {
          suggestion.classList.add("top-artist-track");
          suggestion.setAttribute("aria-label", `${track.title} by ${track.artist}, your top artist pick`);
        } else {
          suggestion.setAttribute("aria-label", `${track.title} by ${track.artist}`);
        }
        suggestion.addEventListener("click", () => selectTrack(track));
        list.append(suggestion);
      });

      section.append(heading, list);
      genreRecommendations.append(section);
    });
  };

  const loadDisplayCounts = (user) => {
    displayCountsStorageKey = displayCountsKeyForUser(user);
    let stored;
    try {
      stored = localStorage.getItem(displayCountsStorageKey);
    } catch (error) {
      throw new Error(`Your listening preferences are unavailable in this browser: ${error.message}`);
    }

    if (!stored) {
      displayCounts = {};
    } else {
      try {
        displayCounts = JSON.parse(stored);
      } catch {
        throw new Error("Saved listening preferences could not be read. Clear this browser's Moodify data and try again.");
      }
      if (!displayCounts || typeof displayCounts !== "object" || Array.isArray(displayCounts)
        || Object.values(displayCounts).some((count) => !Number.isSafeInteger(count) || count < 0)) {
        throw new Error("Saved listening preferences have an invalid format.");
      }
      displayCounts = Object.fromEntries(
        Object.entries(displayCounts).filter(([id]) => musicTracks.some((track) => track.id === id)),
      );
    }

    playCount.textContent = String(Object.values(displayCounts).reduce((total, count) => total + count, 0));
    renderArtistRecommendations();
  };

  const loadLikes = (user) => {
    likesStorageKey = likeKeyForUser(user);
    let stored;
    try {
      stored = localStorage.getItem(likesStorageKey);
    } catch (error) {
      throw new Error(`Liked songs are unavailable in this browser: ${error.message}`);
    }

    if (!stored) {
      likes = new Set();
    } else {
      let ids;
      try {
        ids = JSON.parse(stored);
      } catch {
        throw new Error("Saved liked songs could not be read. Clear this browser's Moodify data and try again.");
      }
      if (!Array.isArray(ids) || ids.some((id) => typeof id !== "string")) {
        throw new Error("Saved liked songs have an invalid format.");
      }
      likes = new Set(ids.filter((id) => musicTracks.some((track) => track.id === id)));
    }

    likedCount.textContent = String(likes.size);
    renderPlaylist();
    updatePlayerLike();
  };

  const renderMoodOptions = () => {
    const moods = ["", ...new Set(musicTracks.flatMap(trackMoods))];
    moodOptions.replaceChildren();
    moods.forEach((mood) => {
      const option = document.createElement("button");
      option.className = "mood-option";
      option.type = "button";
      option.textContent = mood ? formatMood(mood) : "All moods";
      option.setAttribute("aria-pressed", String(mood === selectedMood));
      option.addEventListener("click", () => {
        selectedMood = mood;
        currentMoodLabel.textContent = mood ? formatMood(mood).toLocaleUpperCase() : "ALL MOODS";
        renderDiscovery();
        renderMoodOptions();
        renderPlaylist();
      });
      moodOptions.append(option);
    });
  };

  const renderDiscovery = (shuffle = false) => {
    const candidates = musicTracks.filter((track) => !selectedMood || trackMoods(track).includes(selectedMood));
    if (candidates.length === 0) {
      discoveryTrack = null;
      discoveryTrackTitle.textContent = "No track for this vibe yet";
      discoveryTrackArtist.textContent = "Try another mood to discover a song.";
      displayDiscoveryButton.disabled = true;
      return;
    }

    const choices = shuffle && candidates.length > 1
      ? candidates.filter((track) => track.id !== discoveryTrack?.id)
      : candidates;
    if (!discoveryTrack || !candidates.some((track) => track.id === discoveryTrack.id) || shuffle) {
      discoveryTrack = choices[Math.floor(Math.random() * choices.length)];
    }
    discoveryMood.textContent = selectedMood
      ? `A little ${selectedMood.toLocaleLowerCase()} for your soundtrack.`
      : "A hand-picked sample, just for your next listening moment.";
    discoveryTrackTitle.textContent = discoveryTrack.title;
    discoveryTrackArtist.textContent = `${discoveryTrack.artist} · ${discoveryTrack.genre}`;
    displayDiscoveryButton.disabled = false;
  };

  const displayUser = (user) => {
    const displayName = user.name || "Music lover";
    loadLikes(user);
    loadDisplayCounts(user);
    query("#welcome-name").textContent = displayName;
    query(".profile strong").textContent = displayName;
    query(".profile small").textContent = user.type === "guest" ? "Guest session" : "Free account";
    query("#signout-mobile").textContent = displayName.trim().slice(0, 1).toUpperCase() || "♫";
    query("#signout-mobile").setAttribute("aria-label", `Sign out ${displayName}`);
    authScreen.hidden = true;
    document.body.classList.add("authenticated");
    history.replaceState(null, "", `${location.pathname}${location.search}#home`);
  };

  const showLogin = () => {
    document.body.classList.remove("authenticated");
    authScreen.hidden = false;
    history.replaceState(null, "", `${location.pathname}${location.search}`);
    query("#login-email").focus();
    setMessage("");
  };

  const saveSession = (user) => {
    writeStorage(sessionKey, user);
    displayUser(user);
  };

  const handleSubmit = (form, handler) => {
    form.addEventListener("submit", async (event) => {
      event.preventDefault();
      const submit = query('[type="submit"]', form);
      submit.disabled = true;
      setMessage("");
      try {
        await handler(new FormData(form));
      } catch (error) {
        setMessage(error.message || "Moodify couldn't complete your request. Please try again.");
      } finally {
        submit.disabled = false;
      }
    });
  };

  handleSubmit(loginForm, async (form) => {
    const email = String(form.get("email") || "").trim().toLowerCase();
    const password = String(form.get("password") || "");
    if (!query("#login-email").validity.valid) throw new Error("Enter a valid email address.");
    if (!password) throw new Error("Enter your password.");

    const account = readAccounts().find((item) => item.email === email);
    if (!account) throw new Error("That email or password is not correct.");
    const hash = await hashPassword(password, saltFromHex(account.salt));
    if (!constantTimeEqual(hash, account.passwordHash)) {
      throw new Error("That email or password is not correct.");
    }

    saveSession({ type: "account", accountId: account.id, name: account.name });
    loginForm.reset();
  });

  handleSubmit(signupForm, async (form) => {
    const name = String(form.get("name") || "").trim();
    const email = String(form.get("email") || "").trim().toLowerCase();
    const password = String(form.get("password") || "");
    const confirmation = String(form.get("confirm") || "");

    if (name.length < 2) throw new Error("Enter a name with at least 2 characters.");
    if (!query("#signup-email").validity.valid) throw new Error("Enter a valid email address.");
    if (password.length < 8) throw new Error("Your password must be at least 8 characters.");
    if (password !== confirmation) throw new Error("Those passwords do not match.");

    const accounts = readAccounts();
    if (accounts.some((account) => account.email === email)) {
      throw new Error("An account with that email already exists. Try logging in.");
    }

    const salt = crypto.getRandomValues(new Uint8Array(16));
    const account = {
      id: hex(crypto.getRandomValues(new Uint8Array(16))),
      name,
      email,
      salt: hex(salt),
      passwordHash: await hashPassword(password, salt),
    };
    writeStorage(accountsKey, [...accounts, account]);
    saveSession({ type: "account", accountId: account.id, name: account.name });
    signupForm.reset();
  });

  for (const link of guestLinks) {
    link.addEventListener("click", (event) => {
      event.preventDefault();
      try {
        saveSession({ type: "guest", name: "Music lover" });
      } catch (error) {
        setMessage(error.message);
      }
    });
  }

  for (const mode of document.querySelectorAll(".auth-mode")) {
    mode.addEventListener("change", () => setMessage(""));
  }

  for (const signOut of [query("#signout"), query("#signout-mobile")]) {
    signOut.addEventListener("click", () => {
      try {
        localStorage.removeItem(sessionKey);
        loginForm.reset();
        signupForm.reset();
        query("#auth-login").checked = true;
        showLogin();
      } catch (error) {
        setMessage(`Moodify couldn't sign out in this browser: ${error.message}`);
      }
    });
  }

  const restoreSession = () => {
    let stored;
    try {
      stored = localStorage.getItem(sessionKey);
    } catch (error) {
      setMessage(`Saved sign-in is unavailable in this browser: ${error.message}`);
      return;
    }
    if (!stored) return;

    let session;
    try {
      session = JSON.parse(stored);
    } catch {
      setMessage("Saved sign-in data could not be read. Please log in again.");
      return;
    }

    if (session?.type === "guest" && typeof session.name === "string") {
      try {
        displayUser(session);
      } catch (error) {
        setMessage(error.message);
      }
      return;
    }
    if (session?.type !== "account" || typeof session.accountId !== "string") {
      setMessage("Saved sign-in data is invalid. Please log in again.");
      return;
    }

    let account;
    try {
      account = readAccounts().find((item) => item.id === session.accountId);
    } catch (error) {
      setMessage(error.message);
      return;
    }
    if (!account) {
      setMessage("That saved account could not be found. Please log in again.");
      return;
    }
    try {
      displayUser({ type: "account", accountId: account.id, name: account.name });
    } catch (error) {
      setMessage(error.message);
    }
  };

  const selectTrack = (track, row = null) => {
    selectedTrackId = track.id;
    const nextCounts = { ...displayCounts, [track.id]: (displayCounts[track.id] || 0) + 1 };
    try {
      writeStorage(displayCountsStorageKey, nextCounts);
      displayCounts = nextCounts;
    } catch (error) {
      catalogMessage.textContent = error.message || "Moodify couldn't update your listening preferences.";
    }

    catalogRows.querySelectorAll("tr[data-song-id]").forEach((item) => {
      item.classList.toggle("selected", item === row || item.dataset.songId === track.id);
    });
    query("#player-track-title").textContent = track.title;
    query("#player-track-artist").textContent = `${track.artist} · display only`;
    query("#player-duration").textContent = track.duration;
    query("#player-message").textContent = "Display only · no audio playback";
    playCount.textContent = String(Object.values(displayCounts).reduce((total, count) => total + count, 0));
    updatePlayerLike();
    renderArtistRecommendations();
  };

  const renderPlaylist = () => {
    const search = searchTerm;
    const visibleTracks = musicTracks.filter((track) => {
      const matchesSearch = trackMatchesSearch(track, search);
      return matchesSearch && (!likedOnly || likes.has(track.id))
        && (!selectedMood || trackMoods(track).includes(selectedMood));
    });

    catalogRows.replaceChildren();
    catalogCount.textContent = `${visibleTracks.length} of ${musicTracks.length} songs`;
    likedFilter.setAttribute("aria-pressed", String(likedOnly));

    visibleTracks.forEach((track, index) => {
      const row = document.createElement("tr");
      row.dataset.songId = track.id;
      row.tabIndex = 0;
      row.setAttribute("aria-label", `Display ${track.title} by ${track.artist}`);

      const info = document.createElement("div");
      info.className = "track";
      const thumb = document.createElement("span");
      thumb.className = `thumb art-${track.art}`;
      thumb.textContent = "♫";
      const name = document.createElement("span");
      name.className = "track-name";
      const title = document.createElement("button");
      title.className = "track-select";
      title.type = "button";
      title.textContent = track.title;
      const artist = document.createElement("small");
      artist.textContent = track.artist;
      name.append(title, artist);
      info.append(thumb, name);

      const like = document.createElement("button");
      like.className = "heart";
      like.type = "button";
      updateLikeButton(like, track);
      like.addEventListener("click", (event) => {
        event.stopPropagation();
        try {
          toggleLike(track);
          catalogMessage.textContent = "Sample tracks only · no audio is connected.";
        } catch (error) {
          catalogMessage.textContent = error.message || "Moodify couldn't update your liked songs.";
        }
      });

      row.append(
        makeCell(String(index + 1).padStart(2, "0")),
        makeCell(info),
        makeCell(track.album, "album-col"),
        makeCell(track.moodDisplay, "mood-tag"),
        makeCell(track.duration, "time"),
        makeCell(like),
      );
      row.classList.toggle("selected", track.id === selectedTrackId);
      row.addEventListener("click", () => selectTrack(track, row));
      row.addEventListener("keydown", (event) => {
        if (event.target === row && (event.key === "Enter" || event.key === " ")) {
          event.preventDefault();
          selectTrack(track, row);
        }
      });
      title.addEventListener("click", (event) => {
        event.stopPropagation();
        selectTrack(track, row);
      });
      catalogRows.append(row);
    });

    if (visibleTracks.length === 0) {
      const row = document.createElement("tr");
      const emptyMessage = likedOnly && likes.size === 0
        ? "No liked songs yet. Tap a heart to add one."
        : selectedMood
          ? `No songs match ${selectedMood}. Try another mood or clear the mood filter.`
          : "No sample songs match that search.";
      const cell = makeCell(emptyMessage);
      cell.colSpan = 6;
      row.append(cell);
      catalogRows.append(row);
    }
  };

  const makeCell = (content, className = "") => {
    const cell = document.createElement("td");
    if (className) cell.className = className;
    if (content instanceof Node) cell.append(content);
    else cell.textContent = content;
    return cell;
  };

  const toggleLike = (track) => {
    const nextLikes = new Set(likes);
    if (nextLikes.has(track.id)) nextLikes.delete(track.id);
    else nextLikes.add(track.id);

    writeStorage(likesStorageKey, [...nextLikes]);
    likes = nextLikes;
    likedCount.textContent = String(likes.size);
    renderPlaylist();
    updatePlayerLike();
    renderArtistRecommendations();
  };

  searchInput.addEventListener("input", () => {
    searchTerm = searchInput.value.trim();
    renderPlaylist();
  });
  surpriseTrackButton.addEventListener("click", () => renderDiscovery(true));
  displayDiscoveryButton.addEventListener("click", () => {
    if (!discoveryTrack) return;
    selectTrack(discoveryTrack);
    query("#daily-mix").scrollIntoView({ behavior: "smooth", block: "start" });
  });
  likedFilter.addEventListener("click", () => {
    likedOnly = !likedOnly;
    renderPlaylist();
  });
  playerLike.addEventListener("click", () => {
    const track = musicTracks.find((item) => item.id === selectedTrackId);
    if (!track) return;
    try {
      toggleLike(track);
      catalogMessage.textContent = "Sample tracks only · no audio is connected.";
    } catch (error) {
      catalogMessage.textContent = error.message || "Moodify couldn't update your liked songs.";
    }
  });

  renderMoodOptions();
  renderPlaylist();
  restoreSession();

  loadMusicTracks().then((tracks) => {
    musicTracks = tracks;
    renderDiscovery();
    renderMoodOptions();
    renderPlaylist();
    restoreSession();
  }).catch((error) => {
    catalogMessage.textContent = `Couldn't load the shared song catalog: ${error.message} Start Moodify through a local web server and try again.`;
  });
})();

