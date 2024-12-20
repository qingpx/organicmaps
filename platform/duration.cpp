#include "duration.hpp"

/// @todo(KK): move the formatting code from the platform namespace
namespace platform
{
namespace
{
using std::chrono::duration_cast, std::chrono::seconds, std::chrono::minutes, std::chrono::hours, std::chrono::days;

static constexpr std::string_view kNoSpace = "";

unsigned long SecondsToUnits(seconds duration, Duration::Units unit)
{
  switch (unit)
  {
    case Duration::Units::Days: return duration_cast<days>(duration).count();
    case Duration::Units::Hours: return duration_cast<hours>(duration).count();
    case Duration::Units::Minutes: return duration_cast<minutes>(duration).count();
    default: UNREACHABLE();
  }
}

seconds UnitsToSeconds(long value, Duration::Units unit)
{
  switch (unit)
  {
    case Duration::Units::Days: return days(value);
    case Duration::Units::Hours: return hours(value);
    case Duration::Units::Minutes: return minutes(value);
    default: UNREACHABLE();
  }
}

std::string_view GetUnitSeparator(Locale const & locale)
{
  static constexpr auto kEmptyNumberUnitSeparatorLocales = std::array
  {
    "en", "de", "fr", "he", "fa", "ja", "ko", "mr", "th", "tr", "vi", "zh"
  };
  bool const isEmptySeparator = std::find(std::begin(kEmptyNumberUnitSeparatorLocales), std::end(kEmptyNumberUnitSeparatorLocales), locale.m_language) != std::end(kEmptyNumberUnitSeparatorLocales);
  return isEmptySeparator ? kNoSpace : kNarrowNonBreakingSpace;
}

std::string_view GetUnitsGroupingSeparator(Locale const & locale)
{
  static constexpr auto kEmptyGroupingSeparatorLocales = std::array
  {
    "ja", "zh"
  };
  bool const isEmptySeparator = std::find(std::begin(kEmptyGroupingSeparatorLocales), std::end(kEmptyGroupingSeparatorLocales), locale.m_language) != std::end(kEmptyGroupingSeparatorLocales);
  return isEmptySeparator ? kNoSpace : kNonBreakingSpace;
}

constexpr bool IsUnitsOrderValid(std::initializer_list<Duration::Units> units) {
  auto it = units.begin();
  auto prev = *it;
  ++it;
  for (; it != units.end(); ++it) {
    if (static_cast<int>(*it) <= static_cast<int>(prev))
      return false;
    prev = *it;
  }
  return true;
}
}

Duration::Duration(unsigned long seconds) : m_seconds(seconds)
{}

std::string Duration::GetLocalizedString(std::initializer_list<Units> units, Locale const & locale) const
{
  return GetString(std::move(units), GetUnitSeparator(locale), GetUnitsGroupingSeparator(locale));
}

std::string Duration::GetPlatformLocalizedString() const
{
  static auto const kCurrentUnitSeparator = GetUnitSeparator(GetCurrentLocale());
  static auto const kCurrentGroupingSeparator = GetUnitsGroupingSeparator(GetCurrentLocale());
  return GetString({Units::Days, Units::Hours, Units::Minutes}, kCurrentUnitSeparator, kCurrentGroupingSeparator);
}

std::string Duration::GetString(std::initializer_list<Units> units, std::string_view unitSeparator, std::string_view groupingSeparator) const
{
  ASSERT(units.size(), ());
  ASSERT(IsUnitsOrderValid(units), ());

  if (m_seconds.count() == 0)
    return std::to_string(0U).append(unitSeparator).append(GetUnitsString(Units::Minutes));

  std::string formattedTime;
  seconds remainingSeconds = m_seconds;

  for (auto const unit : units)
  {
    const unsigned long unitsCount = SecondsToUnits(remainingSeconds, unit);
    if (unitsCount > 0)
    {
      if (!formattedTime.empty())
        formattedTime.append(groupingSeparator);
      formattedTime.append(std::to_string(unitsCount).append(unitSeparator).append(GetUnitsString(unit)));
      remainingSeconds -= UnitsToSeconds(unitsCount, unit);
    }
  }
  return formattedTime;
}

std::string Duration::GetUnitsString(Units unit)
{
  constexpr std::string_view kStringsMinute = "minute";
  constexpr std::string_view kStringsHour = "hour";
  constexpr std::string_view kStringsDay = "day";

  switch (unit)
  {
    case Units::Minutes: return platform::GetLocalizedString(std::string(kStringsMinute));
    case Units::Hours: return platform::GetLocalizedString(std::string(kStringsHour));
    case Units::Days: return platform::GetLocalizedString(std::string(kStringsDay));
    default: UNREACHABLE();
  }
}

std::string DebugPrint(Duration::Units units)
{
  switch (units)
  {
    case Duration::Units::Days: return "d";
    case Duration::Units::Hours: return "h";
    case Duration::Units::Minutes: return "m";
    default: UNREACHABLE();
  }
}
}  // namespace platform
