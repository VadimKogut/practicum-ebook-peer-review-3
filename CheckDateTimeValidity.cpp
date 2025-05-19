#include <array>
#include <stdexcept>
#include <string>

using namespace std;

struct DateTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

namespace DateTimeValidation {

namespace {
    constexpr int MIN_YEAR = 1;
    constexpr int MAX_YEAR = 9999;
    constexpr int MIN_MONTH = 1;
    constexpr int MAX_MONTH = 12;
    constexpr int MIN_DAY = 1;
    constexpr int MIN_HOUR = 0;
    constexpr int MAX_HOUR = 23;
    constexpr int MIN_MINUTE = 0;
    constexpr int MAX_MINUTE = 59;
    constexpr int MIN_SECOND = 0;
    constexpr int MAX_SECOND = 59;
}

bool IsLeapYear(int year) {
    return (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
}

int GetMaxDayInMonth(int month, bool is_leap_year) {
    constexpr array<int, 12> month_lengths = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    return month == 2 ? 28 + is_leap_year : month_lengths[month - 1];
}

void ValidateYear(int year) {
    if (year < MIN_YEAR || year > MAX_YEAR) {
        throw domain_error("Invalid year value: " + to_string(year));
    }
}

void ValidateMonth(int month) {
    if (month < MIN_MONTH || month > MAX_MONTH) {
        throw domain_error("Invalid month value: " + to_string(month));
    }
}

void ValidateDay(int day, int month, int year) {
    if (day < MIN_DAY) {
        throw domain_error("Day is too small: " + to_string(day));
    }
    
    const bool is_leap = IsLeapYear(year);
    const int max_day = GetMaxDayInMonth(month, is_leap);
    
    if (day > max_day) {
        throw domain_error("Day is too big for this month: " + to_string(day));
    }
}

void ValidateHour(int hour) {
    if (hour < MIN_HOUR || hour > MAX_HOUR) {
        throw domain_error("Invalid hour value: " + to_string(hour));
    }
}

void ValidateMinute(int minute) {
    if (minute < MIN_MINUTE || minute > MAX_MINUTE) {
        throw domain_error("Invalid minute value: " + to_string(minute));
    }
}

void ValidateSecond(int second) {
    if (second < MIN_SECOND || second > MAX_SECOND) {
        throw domain_error("Invalid second value: " + to_string(second));
    }
}

} // namespace DateTimeValidation

void CheckDateTimeValidity(const DateTime& dt) {
    using namespace DateTimeValidation;
    
    ValidateYear(dt.year);
    ValidateMonth(dt.month);
    ValidateDay(dt.day, dt.month, dt.year);
    ValidateHour(dt.hour);
    ValidateMinute(dt.minute);
    ValidateSecond(dt.second);
}
