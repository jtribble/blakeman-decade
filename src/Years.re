let ensureFocusedRowIsScrolled =
    (~yearsRef, ~scrollLeftByYear, ~focusedRowIndex) =>
  ReactVirtualized.Grid.scrollToCell(
    yearsRef^ |> Belt.Option.getExn,
    {
      "columnIndex":
        Belt.Map.String.getExn(
          scrollLeftByYear,
          ImageMetadata.years[focusedRowIndex],
        )
        |> (scrollLeft => scrollLeft /. 450.0)
        |> int_of_float,
      "rowIndex": focusedRowIndex,
    },
  );

type state = {
  audio: Audio.t,
  currentSong: SongMetadata.song,
  durationByYear: Belt.Map.String.t(float),
  focusedRowIndex: int,
  intervalId: int,
  isMuted: bool,
  lastPlaySongTime: float,
  lightboxPhoto: option((string, int)),
  refByYears: ref(Belt.Map.String.t(option(ReasonReact.reactRef))),
  yearsRef: ref(option(ReasonReact.reactRef)),
  scrollLeftByYear: Belt.Map.String.t(float),
  scrollY: float,
  windowHeight: float,
  windowWidth: float,
};

let setYearsRef = (theRef, {ReasonReact.state}) =>
  state.yearsRef := Js.Nullable.toOption(theRef);

let setYearRef = (year, theRef, {ReasonReact.state}) =>
  state.refByYears :=
    state.refByYears^
    |. Belt.Map.String.set(year, theRef |> Js.Nullable.toOption);

let getCurrentSong = ({durationByYear, focusedRowIndex}) =>
  ImageMetadata.years[focusedRowIndex]
  |> (
    year =>
      SongMetadata.getSong(
        ~year,
        ~duration=
          durationByYear
          |. Belt.Map.String.getExn(year)
          |> (mills => mills /. 1000.0),
      )
  );

let updateDurationByYear =
    ({durationByYear, focusedRowIndex, lastPlaySongTime}) =>
  durationByYear
  |. Belt.Map.String.update(ImageMetadata.years[focusedRowIndex], y =>
       Belt.Option.map(y, duration =>
         duration +. (Js.Date.now() -. lastPlaySongTime)
       )
     );

let leftToIndex = (left, rowIndex) =>
  ImageMetadata.countByYear
  |. Belt.Map.String.getExn(ImageMetadata.years[rowIndex])
  |. Belt.Array.makeBy(columnIndex =>
       Photo.getColumnWidth(rowIndex, columnIndex)
     )
  |. Belt.Array.reduce((0.0, 0), ((sum, i), width) =>
       sum +. width > left ? (sum, i) : (sum +. width, i + 1)
     )
  |> (((_, i)) => i + 1);

type action =
  | FocusRow(int)
  | KeyDown(int)
  | Mute
  | PauseSong
  | PlaySong
  | SelectNextLightboxPhoto
  | SelectPrevLightboxPhoto
  | SetAudio(Audio.t)
  | SetIntervalId(int)
  | SetLightboxPhoto(option((string, int)))
  | SetSong(SongMetadata.song)
  | SetWindowSize
  | SetScrollLeft(string, float)
  | UnMute
  | UpdateSongMaybe;

let component = ReasonReact.reducerComponent("Years");

let playSongForFocusedYear = ({audio, isMuted} as state) => {
  let (song, path, time) = getCurrentSong(state);
  audio |> Audio.pause;
  let audio = Audio.init(path);
  if (isMuted) {
    audio |> Audio.mute;
  };
  Audio.setCurrentTime(audio, time);
  Audio.play(audio);
  (song, audio);
};

let make = _children => {
  ...component,
  initialState: () => {
    audio:
      SongMetadata.getSong(~year=ImageMetadata.years[0], ~duration=0.0)
      |> (((_, path, _)) => path)
      |> Audio.init,
    currentSong:
      SongMetadata.getSong(~year=ImageMetadata.years[0], ~duration=0.0)
      |> (((song, _, _)) => song),
    durationByYear:
      ImageMetadata.years
      |> Array.map(year => (year, 0.0))
      |> Belt.Map.String.fromArray,
    focusedRowIndex: 0,
    intervalId: 0,
    isMuted: true,
    lastPlaySongTime: Js.Date.now(),
    lightboxPhoto: None,
    refByYears:
      ImageMetadata.years
      |> Array.map(year => (year, None))
      |> Belt.Map.String.fromArray
      |> ref,
    scrollLeftByYear:
      ImageMetadata.years
      |> Array.map(year => (year, 0.0))
      |> Belt.Map.String.fromArray,
    scrollY: Window.pageYOffset,
    windowHeight: Window.innerHeight,
    windowWidth: Window.innerWidth,
    yearsRef: ref(None),
  },
  reducer: (action, state) =>
    switch (action) {
    | PauseSong =>
      ReasonReact.UpdateWithSideEffects(
        {...state, durationByYear: updateDurationByYear(state)},
        (self => self.state.audio |> Audio.pause),
      )
    | PlaySong =>
      ReasonReact.UpdateWithSideEffects(
        {...state, lastPlaySongTime: Js.Date.now()},
        (
          ({state, send}) =>
            playSongForFocusedYear(state)
            |. (
              ((song, audio)) => {
                send(SetSong(song));
                send(SetAudio(audio));
              }
            )
        ),
      )

    | Mute =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isMuted: true},
        (self => self.state.audio |> Audio.mute),
      )
    | UnMute =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isMuted: false},
        (({state: {audio}}) => audio |. Audio.unMute),
      )
    | FocusRow(index) =>
      ReasonReact.Update({...state, focusedRowIndex: index})
    | SetAudio(audio) => ReasonReact.Update({...state, audio})
    | SetIntervalId(id) => ReasonReact.Update({...state, intervalId: id})
    | SelectNextLightboxPhoto =>
      ReasonReact.Update({
        ...state,
        lightboxPhoto:
          state.lightboxPhoto
          |. Belt.Option.map(((year, rowIndex)) =>
               (
                 year,
                 Js.Math.min_int(
                   ImageMetadata.countByYear
                   |. Belt.Map.String.getExn(year)
                   |> (x => x - 1),
                   rowIndex + 1,
                 ),
               )
             ),
      })
    | SelectPrevLightboxPhoto =>
      ReasonReact.Update({
        ...state,
        lightboxPhoto:
          state.lightboxPhoto
          |. Belt.Option.map(((year, rowIndex)) =>
               (year, Js.Math.max_int(0, rowIndex - 1))
             ),
      })
    | SetLightboxPhoto(photoInfo) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          lightboxPhoto: photoInfo,
          focusedRowIndex:
            photoInfo
            |. Belt.Option.map(((year, _)) =>
                 year |. ImageMetadata.yearToRowIndex
               )
            |. Belt.Option.getWithDefault(state.focusedRowIndex),
        },
        (
          ({state: {lightboxPhoto}, send}) =>
            (
              lightboxPhoto |. Belt.Option.isSome,
              state.lightboxPhoto |. Belt.Option.isSome,
            )
            |> (
              ((hasLightboxPhoto, didHaveLightBoxPhoto)) =>
                switch (hasLightboxPhoto, didHaveLightBoxPhoto) {
                | (true, false) => send(PlaySong)
                | (false, true) => send(PauseSong)
                | _ => ()
                }
            )
        ),
      )
    | SetSong(song) => ReasonReact.Update({...state, currentSong: song})
    | SetWindowSize =>
      ReasonReact.Update({
        ...state,
        windowHeight: Window.innerHeight,
        windowWidth: Window.innerWidth,
      })
    | SetScrollLeft(year, scroll) =>
      ReasonReact.Update({
        ...state,
        scrollLeftByYear:
          Belt.Map.String.set(state.scrollLeftByYear, year, scroll),
      })
    | KeyDown(keycode) =>
      switch (keycode) {
      | 40 =>
        /* Down */
        state.lightboxPhoto |> Belt.Option.isSome ?
          ReasonReact.NoUpdate :
          ReasonReact.SideEffects(
            (
              self => {
                let rowToFocus =
                  Js.Math.min_int(
                    ImageMetadata.years |> Array.length |> (x => x - 1),
                    self.state.focusedRowIndex + 1,
                  );
                ensureFocusedRowIsScrolled(
                  ~yearsRef=self.state.yearsRef,
                  ~scrollLeftByYear=self.state.scrollLeftByYear,
                  ~focusedRowIndex=rowToFocus,
                );
                self.send(FocusRow(rowToFocus));
              }
            ),
          )
      | 38 =>
        /* Up */
        state.lightboxPhoto |> Belt.Option.isSome ?
          ReasonReact.NoUpdate :
          ReasonReact.SideEffects(
            (
              self => {
                let rowToFocus =
                  Js.Math.max_int(0, self.state.focusedRowIndex - 1);
                ensureFocusedRowIsScrolled(
                  ~yearsRef=self.state.yearsRef,
                  ~scrollLeftByYear=self.state.scrollLeftByYear,
                  ~focusedRowIndex=rowToFocus,
                );
                self.send(FocusRow(rowToFocus));
              }
            ),
          )
      | 37 =>
        /* Left */
        ReasonReact.SideEffects(
          (
            self =>
              state.lightboxPhoto |> Belt.Option.isSome ?
                self.send(SelectPrevLightboxPhoto) :
                ScrollButton.scrollRow(
                  ~refByYears=self.state.refByYears,
                  ~year=ImageMetadata.years[self.state.focusedRowIndex],
                  ~scrollLeftByYear=self.state.scrollLeftByYear,
                  ~scrollDelta=self.state.windowWidth *. (-0.7),
                )
          ),
        )
      | 39 =>
        /* Right */
        ReasonReact.SideEffects(
          (
            self =>
              state.lightboxPhoto |> Belt.Option.isSome ?
                self.send(SelectNextLightboxPhoto) :
                ScrollButton.scrollRow(
                  ~refByYears=self.state.refByYears,
                  ~year=ImageMetadata.years[self.state.focusedRowIndex],
                  ~scrollLeftByYear=self.state.scrollLeftByYear,
                  ~scrollDelta=self.state.windowWidth *. 0.7,
                )
          ),
        )
      | 27 =>
        /* Escape */
        ReasonReact.SideEffects((self => self.send(SetLightboxPhoto(None))))
      | 13
      | 32 =>
        /* Enter or Space */
        let year = ImageMetadata.years[state.focusedRowIndex];
        ReasonReact.SideEffects(
          (
            ({send}) =>
              (
                state.lightboxPhoto |> Belt.Option.isSome ?
                  None :
                  Some((
                    year,
                    state.scrollLeftByYear
                    |. Belt.Map.String.getExn(year)
                    |. leftToIndex(state.focusedRowIndex),
                  ))
              )
              |. SetLightboxPhoto
              |. send
          ),
        );
      | _ => ReasonReact.NoUpdate
      }
    | UpdateSongMaybe =>
      ReasonReact.SideEffects(
        (
          self => {
            let (_, newSongPath, _) =
              updateDurationByYear(self.state)
              |> (
                newDurationByYear =>
                  getCurrentSong({
                    ...self.state,
                    durationByYear: newDurationByYear,
                  })
              );
            let (_, oldSongPath, _) = getCurrentSong(self.state);
            if (oldSongPath != newSongPath
                && self.state.lightboxPhoto
                |. Belt.Option.isSome) {
              self.send(PauseSong);
              self.send(PlaySong);
            };
          }
        ),
      )
    },
  subscriptions: self => [
    Sub(
      () =>
        Window.addEventListener(
          "resize",
          () => self.send(SetWindowSize),
          true,
        ),
      () =>
        Window.removeEventListener(
          "resize",
          () => self.send(SetWindowSize),
          true,
        ),
    ),
    Sub(
      () =>
        Body.addEventListener("keydown", event =>
          self.send(KeyDown(event##keyCode))
        ),
      () =>
        Body.removeEventListener("keydown", event =>
          self.send(KeyDown(event##keyCode))
        ),
    ),
    Sub(
      () =>
        self.send(
          SetIntervalId(
            Window.setInterval(() => self.send(UpdateSongMaybe), 1000.0),
          ),
        ),
      () => Window.clearInterval(self.state.intervalId) |> ignore,
    ),
  ],
  render: self =>
    <div>
      <ReactVirtualized.Grid
        cellRenderer=(
          Year.render(
            ~isLightboxOpen=self.state.lightboxPhoto |> Belt.Option.isSome,
            ~width=self.state.windowWidth,
            ~scrollLeftByYear=self.state.scrollLeftByYear,
            ~onClickPhoto=
              yearAndRow => self.send(SetLightboxPhoto(Some(yearAndRow))),
            ~onHoverPhoto=
              index =>
                index != self.state.focusedRowIndex ?
                  self.send(FocusRow(index)) : (),
            ~onScrollLeft=
              (year, scroll) => self.send(SetScrollLeft(year, scroll)),
            ~setRef=year => self.handle(setYearRef(year)),
            ~focusedRowIndex=self.state.focusedRowIndex,
            ~refByYears=self.state.refByYears,
          )
        )
        className="years"
        columnCount=1
        columnWidth=self.state.windowWidth
        height=self.state.windowHeight
        ref=(self.handle(setYearsRef))
        rowCount=(ImageMetadata.years |> Array.length)
        rowHeight=Constants.rowHeight
        scrollToAlignment="center"
        width=self.state.windowWidth
      />
      (
        self.state.lightboxPhoto
        |. Belt.Option.map(((year, rowIndex)) =>
             <Lightbox
               close=(_ => self.send(SetLightboxPhoto(None)))
               height=self.state.windowHeight
               artist=self.state.currentSong##artist
               song=self.state.currentSong##song
               isMuted=self.state.isMuted
               mute=(_ => self.send(Mute))
               unmute=(_ => self.send(UnMute))
               path=(
                 year
                 |> ImageMetadata.getMediumImagePaths
                 |. Belt.Array.get(rowIndex)
                 |. Belt.Option.getWithDefault("")
               )
               nextPhoto=(
                 e => {
                   ReactEventRe.Mouse.stopPropagation(e);
                   self.send(SelectNextLightboxPhoto);
                 }
               )
               prevPhoto=(_ => self.send(SelectPrevLightboxPhoto))
               width=self.state.windowWidth
               year
             />
           )
        |. Belt.Option.getWithDefault(ReasonReact.null)
      )
    </div>,
};
