
#include "../util/test.h"
#include "dataframe/impl/dataframe.h"
#include "dataframe/interface.h"

using bool_msg = test::expected_bool_t;
using test::assert;
using test::check;
using test::input;
using test::skip;
using test::skip_error;
using test::throw_;
using test::operator"" _suite;
using test::operator"" _is_true;
using Vizzu::dataframe::cell_value;
using Vizzu::dataframe::na_position;
using Vizzu::dataframe::sort_type;
using interface = Vizzu::dataframe::dataframe_interface;
using record_type = interface::record_type;

struct if_setup
{
	std::vector<const char *> dims{};
	std::vector<const char *> meas{};
	std::vector<std::vector<cell_value>> data{};
	bool copied{};
	std::shared_ptr<interface> _df{
	    Vizzu::dataframe::dataframe::create_new()};

	explicit(false) operator interface *()
	{
		interface *df = _df.get();

		try {
			skip->*static_cast<bool>(df)
			    == "dataframe exists"_is_true;

			for (auto d : dims) df->add_dimension({}, {}, d, {}, {});
			for (auto m : meas) df->add_measure({}, m, {}, {});

			const auto ds = dims.size();
			for (const auto &r : data) df->add_record(r);
			if (copied) {
				df->finalize();
				df = (_df = df->copy(false, false)).get();
			}

			skip->*df->get_dimensions() == dims;
			skip->*df->get_measures() == meas;
			skip->*df->get_record_count() == data.size();

			for (auto r = 0u; r < data.size(); ++r) {
				for (auto d = 0u; d < ds; ++d)
					skip->*df->get_data(r, dims[d]) == data[r][d];

				for (auto m = 0u; m < meas.size(); ++m) {
					if (auto *d =
					        std::get_if<double>(&data[r][m + ds]);
					    d && std::isnan(*d)) {
						auto z = df->get_data(r, meas[m]);
						auto *st = std::get_if<double>(&z);
						skip->*static_cast<bool>(st)
						    == "value is a double"_is_true;
						skip->*std::isnan(*st)
						    == "value is nan"_is_true;
						continue;
					}
					skip->*df->get_data(r, meas[m])
					    == data[r][m + ds];
				}
			}
		}
		catch (skip_error const &) {
			throw;
		}
		catch (std::exception const &e) {
			throw skip_error(
			    std::string{"setup failed: "} + e.what());
		}
		catch (...) {
			throw skip_error("setup failed");
		}

		auto test = test::collection::instance().running_test();
		test->set_latest_location(test->get_test_location());

		return df;
	}
};

static inline const auto empty_input = input{[]
    {
	    return if_setup{};
    },
    "empty input"};

static inline const auto empty_input_copied = input{[]
    {
	    return if_setup{.copied = true};
    },
    "empty input copied"};

static inline const auto one_one_empty = input{[]
    {
	    return if_setup{.dims = {"test_dim"}, .meas = {"test_meas"}};
    },
    "one one empty"};

static inline const auto one_one_empty_copied = input{[]
    {
	    return if_setup{.dims = {"test_dim"},
	        .meas = {"test_meas"},
	        .copied = true};
    },
    "one one empty copied"};

const static auto tests =
    "DataFrame::interface"_suite

    | "construct_empty"
    | empty_input + empty_input_copied <=>
          [](interface *df)
{
	check->*df->get_dimensions().size() == std::size_t{};
	check->*df->get_measures().size() == std::size_t{};
	check->*df->get_record_count() == std::size_t{};

	throw_<&interface::get_data>(df, {}, {});
	throw_<&interface::get_categories>(df, {});
	throw_<&interface::get_min_max>(df, {});
	throw_<&interface::add_series_by_other>(df,
	    {},
	    {""},
	    {[](record_type, cell_value c) -> cell_value
	        {
		        return c;
	        }},
	    {});
	throw_<&interface::set_aggregate>(df, {}, {});
	throw_<&interface::set_sort>(df, {}, {}, {});

	check->*df->get_dimensions().size() == std::size_t{};
	check->*df->get_measures().size() == std::size_t{};
	check->*df->get_record_count() == std::size_t{};
}

    | "add_dimension_and_measure"
    | empty_input + empty_input_copied <=>
          [](interface *df)
{
	constexpr auto nan = std::numeric_limits<double>::quiet_NaN();

	df->add_dimension({{"t2", "t1", "tt3"}},
	    {{0, 0, 2}},
	    "d1",
	    {},
	    {});
	df->add_dimension({{"a"}}, {{0}}, "d0", {}, {});

	df->add_measure({{0.0, 22.5, nan, 6.0}}, "m1", {}, {});
	df->add_measure({{1.0}}, "m2", {}, {});

	assert->*df->get_dimensions() == std::array{"d0", "d1"};
	assert->*df->get_measures() == std::array{"m1", "m2"};

	check->*df->has_na("d0") == "d0 has nav"_is_true;
	check->*df->has_na("d1") == "d1 has nav"_is_true;
	check->*df->has_na("m1") == "m1 has nan"_is_true;
	check->*df->has_na("m2") == "m2 has nan"_is_true;

	check->*df->get_categories("d0") == std::array{"a"};

	assert->*df->get_categories("d1")
	    == std::array{"t2", "t1", "tt3"};

	check->*df->get_min_max("m1") == std::pair{0.0, 22.5};

	check->*df->get_min_max("m2") == std::pair{1.0, 1.0};

	df->finalize();

	assert->*df->get_record_count() == std::size_t{4};

	auto check_nan = [](cell_value const &cell, std::string &&prefix)
	{
		auto str = prefix + " is a double";
		assert->*std::holds_alternative<double>(cell)
		    == bool_msg{str};
		str = prefix + " is nan";
		check->*std::isnan(std::get<double>(cell)) == bool_msg{str};
	};

	auto check_nav = [](cell_value const &cell, std::string &&prefix)
	{
		auto str = prefix + " is a string";
		assert->*std::holds_alternative<std::string_view>(cell)
		    == bool_msg{str};
		str = prefix + " is nav";
		check->*(std::get<std::string_view>(cell).data() == nullptr)
		    == bool_msg{str};
	};

	check->*df->get_data(std::size_t{0}, "m1") == 0.0;
	check->*df->get_data(std::size_t{1}, "m1") == 22.5;
	check_nan(df->get_data(std::size_t{2}, "m1"), "table_20");
	check->*df->get_data(std::size_t{3}, "m1") == 6.0;

	check->*df->get_data(std::size_t{0}, "m2") == 1.0;
	check_nan(df->get_data(std::size_t{1}, "m2"), "table_11");
	check_nan(df->get_data(std::size_t{2}, "m2"), "table_21");
	check_nan(df->get_data(std::size_t{3}, "m2"), "table_31");

	check->*df->get_data(std::size_t{0}, "d0") == "a";
	check_nav(df->get_data(std::size_t{1}, "d0"), "table_12");
	check_nav(df->get_data(std::size_t{2}, "d0"), "table_22");
	check_nav(df->get_data(std::size_t{3}, "d0"), "table_32");

	assert->*df->get_data(std::size_t{0}, "d1") == "t2";
	check->*df->get_data(std::size_t{1}, "d1") == "t2";
	check->*df->get_data(std::size_t{2}, "d1") == "tt3";
	check_nav(df->get_data(std::size_t{3}, "d1"), "table_33");

	check
	            ->*(std::get<std::string_view>(
	                    df->get_data(std::size_t{0}, "d1"))
	                    .data()
	                == df->get_categories("d1")[0].data())
	    == "Not points to the same memory address"_is_true;
}

    | "add_record"
    | one_one_empty + one_one_empty_copied <=>
          [](interface *df)
{
	df->add_record({{"test_dim_val", 2.0}});
	df->add_record({{-1.0, "test_dim_val2"}});

	throw_<&interface::add_record>(df, {});
	throw_<&interface::add_record>(df, {{0.0}});
	throw_<&interface::add_record>(df, {{0.0, 0.0}});
	throw_<&interface::add_record>(df, {{"test", "t"}});
	throw_<&interface::add_record>(df, {{0.0, "test", 0.0}});
	throw_<&interface::add_record>(df, {{0.0, "test", "t"}});

	df->finalize();

	assert->*df->get_record_count() == std::size_t{2};

	check->*df->get_categories("test_dim")
	    == std::array{"test_dim_val", "test_dim_val2"};

	check->*df->get_min_max("test_meas") == std::pair{-1.0, 2.0};

	check->*df->get_data(std::size_t{}, "test_meas") == 2.0;
	check->*df->get_data(std::size_t{}, "test_dim") == "test_dim_val";

	check->*df->get_data(std::size_t{1}, "test_meas") == -1.0;
	check->*df->get_data(std::size_t{1}, "test_dim")
	    == "test_dim_val2";
}

    | "add_series_by_other" |
    [](interface *df = if_setup{{"d1", "d2"},
           {"m1"},
           {{{"dm1", "dm2", 0.0}},
               {{"dm1", "dmX", 1.0}},
               {{"s1", "s2", -1.0}},
               {{"s1", "s3", 3.0}}}})
{
	df->add_series_by_other("m1",
	    "m0",
	    [](auto, cell_value c) -> cell_value
	    {
		    const double *v = std::get_if<double>(&c);
		    assert->*static_cast<bool>(v)
		        == "value is a double"_is_true;

		    return *v * 2;
	    },
	    {});

	assert->*df->get_measures() == std::array{"m0", "m1"};

	check->*df->get_data(std::size_t{0}, "m0") == 0.0;
	check->*df->get_data(std::size_t{1}, "m0") == 2.0;
	check->*df->get_data(std::size_t{2}, "m0") == -2.0;
	check->*df->get_data(std::size_t{3}, "m0") == 6.0;

	df->add_series_by_other("d1",
	    "d15",
	    [](const record_type &r, cell_value c) -> cell_value
	    {
		    auto v = std::get_if<std::string_view>(&c);
		    skip->*static_cast<bool>(v) == "value is string"_is_true;

		    auto oth_v = r.get_value("d2");

		    auto v2 = std::get_if<std::string_view>(&oth_v);
		    skip->*static_cast<bool>(v2) == "value is string"_is_true;

		    thread_local std::string val;
		    val = std::string{*v} + "5" + std::string{*v2};

		    return val;
	    },
	    {});

	assert->*df->get_dimensions() == std::array{"d1", "d15", "d2"};

	check->*df->get_data(std::size_t{0}, "d15") == "dm15dm2";
	check->*df->get_data(std::size_t{1}, "d15") == "dm15dmX";
	check->*df->get_data(std::size_t{2}, "d15") == "s15s2";
	check->*df->get_data(std::size_t{3}, "d15") == "s15s3";
}

    | "remove_series" |
    [](interface *df = if_setup{{"d1", "d2", "d3"},
           {"m1", "m2", "m3"},
           {
               {{"dm1", "dx2", "dm3", 0.0, 0.1, 0.2}},
               {{"dm1", "dx2", "am3", 1.0, 2.1, 3.2}},
               {{"dm2", "dm2", "bm3", 1.0, 1.5, 1.2}},
           }})
{
	df->remove_series({{"m1", "d2", "m3"}});

	assert->*df->get_measures() == std::array{"m2"};
	assert->*df->get_dimensions() == std::array{"d1", "d3"};
	assert->*df->get_record_count() == std::size_t{3};

	check->*df->get_data(std::size_t{2}, "m2") == 1.5;
	check->*df->get_data(std::size_t{0}, "d3") == "dm3";
}

    | "remove_records" |
    [](interface *df = if_setup{{"d1"},
           {"m1"},
           {
               {{"dm0", NAN}},
               {{"dm1", NAN}},
               {{"dm2", NAN}},
               {{"dm3", NAN}},
               {{"dm4", NAN}},
               {{"dm5", NAN}},
               {{"dm6", NAN}},
               {{"dm7", NAN}},
               {{"dm8", 4.2}},
               {{"dm9", NAN}},
           }})
{
	df->remove_records({{0ul, 2ul, 4ul, 5ul, 8ul, 9ul}});

	assert->*df->get_measures() == std::array{"m1"};
	assert->*df->get_dimensions() == std::array{"d1"};
	assert->*df->get_record_count() == std::size_t{4};

	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{2}, "m1")))
	    == "is nan"_is_true;
	check->*df->get_data(std::size_t{0}, "d1") == "dm6";
	check->*df->get_data(std::size_t{1}, "d1") == "dm1";
	check->*df->get_data(std::size_t{2}, "d1") == "dm7";
	check->*df->get_data(std::size_t{3}, "d1") == "dm3";
}

    | "remove_records_filter" |
    [](interface *df = if_setup{{"d1"},
           {"m1"},
           {
               {{"dm0", 5.3}},
               {{"dm1", 2.0}},
               {{"dm2", 3.3}},
               {{"dm3", 10.1}},
               {{"dm4", 88.0}},
               {{"dm5", 2.2}},
               {{"dm6", 7.4}},
               {{"dm7", 0.0}},
               {{"dm8", 4.2}},
               {{"dm9", NAN}},
           }})
{
	df->remove_records(
	    [](record_type r) -> bool
	    {
		    auto v = r.get_value("m1");
		    return *std::get_if<double>(&v) < 5.0;
	    });

	assert->*df->get_record_count() == std::size_t{5};

	check->*df->get_data(std::size_t{0}, "d1") == "dm0";
	check->*df->get_data(std::size_t{1}, "d1") == "dm9";
	check->*df->get_data(std::size_t{2}, "d1") == "dm6";
	check->*df->get_data(std::size_t{3}, "d1") == "dm3";
	check->*df->get_data(std::size_t{4}, "d1") == "dm4";
}

    | "change_data" |
    [](interface *df = if_setup{{"d1"},
           {"m1"},
           {{{"dm0", 5.3}}, {{"dm1", 2.0}}, {{"dm2", 3.3}}}})
{
	df->change_data(std::size_t{1}, "m1", 3.0);
	df->change_data(std::size_t{2}, "d1", "dmX");

	throw_<&interface::change_data>(df,
	    {std::size_t{0}},
	    {"d1"},
	    {NAN});
	throw_<&interface::change_data>(df,
	    {std::size_t{0}},
	    {"m1"},
	    {""});

	assert->*df->get_record_count() == std::size_t{3};

	check->*df->get_data(std::size_t{0}, "m1") == 5.3;
	check->*df->get_data(std::size_t{1}, "m1") == 3.0;
	check->*df->get_data(std::size_t{2}, "m1") == 3.3;

	check->*df->get_data(std::size_t{0}, "d1") == "dm0";
	check->*df->get_data(std::size_t{1}, "d1") == "dm1";
	check->*df->get_data(std::size_t{2}, "d1") == "dmX";
}

    | "fill_na" |
    [](interface *df = if_setup{{"d1"},
           {"m1"},
           {{{"dm0", 5.3}},
               {{std::string_view{nullptr, 0}, 2.0}},
               {{"dm2", NAN}}}})
{
	df->fill_na("m1", 3.0);
	df->fill_na("d1", "dmX");

	assert->*df->get_record_count() == std::size_t{3};

	check->*df->get_data(std::size_t{0}, "m1") == 5.3;
	check->*df->get_data(std::size_t{1}, "m1") == 2.0;
	check->*df->get_data(std::size_t{2}, "m1") == 3.0;

	check->*df->get_data(std::size_t{0}, "d1") == "dm0";
	check->*df->get_data(std::size_t{1}, "d1") == "dmX";
	check->*df->get_data(std::size_t{2}, "d1") == "dm2";
}

    | "aggregate types" |
    [](interface *df = if_setup{{"d1"},
           {"m1"},
           {
               {{"dm0", 5.5}},
               {{"dm0", 2.0}},
               {{"dm0", 3.5}},
               {{"dm0", 10.25}},
               {{"dm0", 88.0}},
               {{"dm1", 3.5}},
               {{"dm1", 7.25}},
               {{"dm1", NAN}},
               {{"dm1", 4.25}},
               {{"dm2", NAN}},
               {{std::string_view{nullptr, 0}, 0.0}},
           }})
{
	df->aggregate_by("d1");
	using enum Vizzu::dataframe::aggregator_type;
	auto &&pure1c = df->set_aggregate({}, count);
	auto &&d1c = df->set_aggregate("d1", count);
	auto &&d1d = df->set_aggregate("d1", distinct);
	auto &&d1e = df->set_aggregate("d1", exists);
	auto &&m1s = df->set_aggregate("m1", sum);
	auto &&m1mi = df->set_aggregate("m1", min);
	auto &&m1ma = df->set_aggregate("m1", max);
	auto &&m1me = df->set_aggregate("m1", mean);
	auto &&m1c = df->set_aggregate("m1", count);
	auto &&m1e = df->set_aggregate("m1", exists);

	auto &&m1t = df->set_aggregate("m1",
	    Vizzu::dataframe::custom_aggregator{{"test"},
	        []() -> Vizzu::dataframe::custom_aggregator::id_type
	        {
		        return std::pair{std::numeric_limits<double>::max(),
		            std::numeric_limits<double>::max()};
	        },
	        [](Vizzu::dataframe::custom_aggregator::id_type &id,
	            cell_value const &cell) -> double
	        {
		        auto &[min, min2] =
		            std::any_cast<std::pair<double, double> &>(id);
		        const double &v = *std::get_if<double>(&cell);
		        if (v < min)
			        min2 = std::exchange(min, v);
		        else if (v < min2)
			        min2 = v;
		        return min2;
	        }});

	df->finalize();

	assert->*df->get_dimensions() == std::array{"d1"};

	assert->*df->get_measures()
	    == std::array{pure1c,
	        d1c,
	        m1c,
	        d1d,
	        d1e,
	        m1e,
	        m1s,
	        m1ma,
	        m1me,
	        m1mi,
	        m1t};

	assert->*df->get_record_count() == std::size_t{4};

	check->*df->get_data(std::size_t{0}, pure1c) == 5.0;
	check->*df->get_data(std::size_t{0}, d1c) == 5.0;
	check->*df->get_data(std::size_t{0}, m1c) == 5.0;
	check->*df->get_data(std::size_t{0}, d1d) == 1.0;
	check->*df->get_data(std::size_t{0}, d1e) == 1.0;
	check->*df->get_data(std::size_t{0}, m1e) == 1.0;
	check->*df->get_data(std::size_t{0}, m1ma) == 88.0;
	check->*df->get_data(std::size_t{0}, m1me) == 21.85;
	check->*df->get_data(std::size_t{0}, m1mi) == 2.0;
	check->*df->get_data(std::size_t{0}, m1s) == 109.25;
	check->*df->get_data(std::size_t{0}, m1t) == 3.5;

	check->*df->get_data(std::size_t{1}, pure1c) == 4.0;
	check->*df->get_data(std::size_t{1}, d1c) == 4.0;
	check->*df->get_data(std::size_t{1}, m1c) == 3.0;
	check->*df->get_data(std::size_t{1}, d1d) == 1.0;
	check->*df->get_data(std::size_t{1}, d1e) == 1.0;
	check->*df->get_data(std::size_t{1}, m1e) == 1.0;
	check->*df->get_data(std::size_t{1}, m1ma) == 7.25;
	check->*df->get_data(std::size_t{1}, m1me) == 5.0;
	check->*df->get_data(std::size_t{1}, m1mi) == 3.5;
	check->*df->get_data(std::size_t{1}, m1s) == 15.0;
	check->*df->get_data(std::size_t{1}, m1t) == 4.25;

	check->*df->get_data(std::size_t{2}, pure1c) == 1.0;
	check->*df->get_data(std::size_t{2}, d1c) == 1.0;
	check->*df->get_data(std::size_t{2}, m1c) == 0.0;
	check->*df->get_data(std::size_t{2}, d1d) == 1.0;
	check->*df->get_data(std::size_t{2}, d1e) == 1.0;
	check->*df->get_data(std::size_t{2}, m1e) == 0.0;
	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{2}, m1ma)))
	    == "is nan"_is_true;
	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{2}, m1me)))
	    == "is nan"_is_true;
	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{2}, m1mi)))
	    == "is nan"_is_true;
	check->*df->get_data(std::size_t{2}, m1s) == 0.0;
	check->*df->get_data(std::size_t{2}, m1t)
	    == std::numeric_limits<double>::max();

	check->*df->get_data(std::size_t{3}, pure1c) == 1.0;
	check->*df->get_data(std::size_t{3}, d1c) == 0.0;
	check->*df->get_data(std::size_t{3}, m1c) == 1.0;
	check->*df->get_data(std::size_t{3}, d1d) == 0.0;
	check->*df->get_data(std::size_t{3}, d1e) == 0.0;
	check->*df->get_data(std::size_t{3}, m1e) == 1.0;
}

    | "aggregate multiple dim" |
    [](interface *df = if_setup{{"d1", "d2", "d3"},
           {"m1"},
           {
               {{"dx0", "dm0", "doa", 5.5}},
               {{"dx0", "dm0", "dob", 2.0}},
               {{"dx0", "dm1", std::string_view{nullptr, 0}, 3.5}},
               {{"dx0", "dm1", "dob", 10.25}},
               {{"dx0", "dm0", "doa", 88.0}},
               {{"dx1", "dm0", "doa", 3.5}},
               {{"dx1", "dm1", std::string_view{nullptr, 0}, 7.25}},
               {{"dx1", "dm2", "doa", NAN}},
               {{"dx1", "dm0", "doa", 4.25}},
               {{"dx2", "dm0", "dob", NAN}},
               {{"dx2", "dm0", "doa", 0.5}},
           }})
{
	df->aggregate_by("d1");
	df->aggregate_by("d2");
	using enum Vizzu::dataframe::aggregator_type;
	auto &&d3c = df->set_aggregate("d3", count);
	auto &&d3d = df->set_aggregate("d3", distinct);

	df->finalize();

	assert->*df->get_dimensions() == std::array{"d1", "d2"};
	assert->*df->get_measures() == std::array{d3c, d3d};

	assert->*df->get_record_count() == std::size_t{6};

	check->*df->get_data(std::size_t{0}, "d1") == "dx0";
	check->*df->get_data(std::size_t{0}, "d2") == "dm0";
	check->*df->get_data(std::size_t{0}, d3c) == 3.0;
	check->*df->get_data(std::size_t{0}, d3d) == 2.0;

	check->*df->get_data(std::size_t{1}, "d1") == "dx0";
	check->*df->get_data(std::size_t{1}, "d2") == "dm1";
	check->*df->get_data(std::size_t{1}, d3c) == 1.0;
	check->*df->get_data(std::size_t{1}, d3d) == 1.0;

	check->*df->get_data(std::size_t{2}, "d1") == "dx1";
	check->*df->get_data(std::size_t{2}, "d2") == "dm0";
	check->*df->get_data(std::size_t{2}, d3c) == 2.0;
	check->*df->get_data(std::size_t{2}, d3d) == 1.0;

	check->*df->get_data(std::size_t{3}, "d1") == "dx1";
	check->*df->get_data(std::size_t{3}, "d2") == "dm1";
	check->*df->get_data(std::size_t{3}, d3c) == 0.0;
	check->*df->get_data(std::size_t{3}, d3d) == 0.0;

	check->*df->get_data(std::size_t{4}, "d1") == "dx1";
	check->*df->get_data(std::size_t{4}, "d2") == "dm2";

	check->*df->get_data(std::size_t{5}, "d1") == "dx2";
	check->*df->get_data(std::size_t{5}, "d2") == "dm0";
	check->*df->get_data(std::size_t{5}, d3c) == 2.0;
	check->*df->get_data(std::size_t{5}, d3d) == 2.0;
}

    | "cannot finalize contains same dim" |
    [](interface *df = if_setup{.dims = {"d1", "d2"},
           .data = {{{"dx0", "dm0"}}, {{"dx0", "dm0"}}}})
{
	throw_<&interface::finalize>(df);
}

    | "sort dimension" |
    [](interface *df = if_setup{{"d1", "d2"},
           {"m1"},
           {
               {{"dx2", "dm2", 5.5}},
               {{"dx1", "dm0", 2.0}},
               {{std::string_view{nullptr, 0}, "dm1", 3.5}},
               {{"dx0", "dm1", 10.25}},
               {{"dx2", "dm0", 88.0}},
               {{"dx1", "dm0", 3.5}},
               {{"dx2", "dm1", 7.25}},
               {{"dx1", "dm2", NAN}},
               {{"dx0", "dm0", 4.25}},
               {{"dx0", "dm0", NAN}},
               {{"dx2", "dm0", 0.5}},
           }})
{
	df->set_sort("d1", {}, {});
	df->set_sort("m1", sort_type::greater, na_position::first);

	// cannot finalize because of duplicated dimensions
	df->remove_records(std::span<interface::record_identifier>{});

	assert->*df->get_record_count() == std::size_t{11};

	assert->*df->get_categories("d1")
	    == std::array{"dx0", "dx1", "dx2"};
	check->*df->get_categories("d2")
	    == std::array{"dm2", "dm0", "dm1"};

	check->*df->get_data(std::size_t{0}, "d1") == "dx0";
	check->*df->get_data(std::size_t{1}, "d1") == "dx0";
	check->*df->get_data(std::size_t{2}, "d1") == "dx0";
	check->*df->get_data(std::size_t{3}, "d1") == "dx1";
	check->*df->get_data(std::size_t{4}, "d1") == "dx1";
	check->*df->get_data(std::size_t{5}, "d1") == "dx1";
	check->*df->get_data(std::size_t{6}, "d1") == "dx2";
	check->*df->get_data(std::size_t{7}, "d1") == "dx2";
	check->*df->get_data(std::size_t{8}, "d1") == "dx2";
	check->*df->get_data(std::size_t{9}, "d1") == "dx2";
	check
	            ->*std::get<std::string_view>(
	                df->get_data(std::size_t{10}, "d1"))
	            .data()
	    == nullptr;

	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{0}, "m1")))
	    == "is nan"_is_true;
	check->*df->get_data(std::size_t{1}, "m1") == 10.25;
	check->*df->get_data(std::size_t{2}, "m1") == 4.25;

	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{3}, "m1")))
	    == "is nan"_is_true;
	check->*df->get_data(std::size_t{4}, "m1") == 3.5;
	check->*df->get_data(std::size_t{5}, "m1") == 2.0;

	check->*df->get_data(std::size_t{6}, "m1") == 88.0;
	check->*df->get_data(std::size_t{7}, "m1") == 7.25;
	check->*df->get_data(std::size_t{8}, "m1") == 5.5;
	check->*df->get_data(std::size_t{9}, "m1") == 0.5;

	check->*df->get_data(std::size_t{10}, "m1") == 3.5;
}

    | "another sort example" |
    [](interface *df = if_setup{{"d1", "d2"},
           {"m1"},
           {
               {{"dx2", "dm2", 88.0}},
               {{"dx1", "dm0", 2.0}},
               {{std::string_view{nullptr, 0}, "dm1", 3.5}},
               {{"dx0", "dm1", 10.25}},
               {{"dx2", "dm8", 5.5}},
               {{"dx1", "dm8", 3.5}},
               {{"dx2", "dm1", 7.25}},
               {{"dx1", "dm2", NAN}},
               {{"dx0", "dm8", 4.25}},
               {{"dx0", "dm0", NAN}},
               {{"dx2", "dm0", 0.5}},
           }})
{
	df->set_sort("d1", sort_type::greater, na_position::first);
	df->set_custom_sort(
	    [](const record_type &lhs, const record_type &rhs)
	    {
		    auto l =
		        (std::get<std::string_view>(lhs.get_value("d2"))[2]
		            - '0')
		        + std::get<double>(lhs.get_value("m1"));
		    auto r =
		        (std::get<std::string_view>(rhs.get_value("d2"))[2]
		            - '0')
		        + std::get<double>(rhs.get_value("m1"));
		    return std::weak_order(l, r);
	    });

	df->finalize();

	assert->*df->get_record_count() == std::size_t{11};

	assert->*df->get_categories("d1")
	    == std::array{"dx2", "dx1", "dx0"};
	check->*df->get_categories("d2")
	    == std::array{"dm2", "dm0", "dm1", "dm8"};

	check
	            ->*std::get<std::string_view>(
	                df->get_data(std::size_t{0}, "d1"))
	            .data()
	    == nullptr;
	check->*df->get_data(std::size_t{1}, "d1") == "dx2";
	check->*df->get_data(std::size_t{2}, "d1") == "dx2";
	check->*df->get_data(std::size_t{3}, "d1") == "dx2";
	check->*df->get_data(std::size_t{4}, "d1") == "dx2";
	check->*df->get_data(std::size_t{5}, "d1") == "dx1";
	check->*df->get_data(std::size_t{6}, "d1") == "dx1";
	check->*df->get_data(std::size_t{7}, "d1") == "dx1";
	check->*df->get_data(std::size_t{8}, "d1") == "dx0";
	check->*df->get_data(std::size_t{9}, "d1") == "dx0";
	check->*df->get_data(std::size_t{10}, "d1") == "dx0";

	check->*df->get_data(std::size_t{0}, "m1") == 3.5;

	check->*df->get_data(std::size_t{1}, "m1") == 0.5;
	check->*df->get_data(std::size_t{2}, "m1") == 7.25;
	check->*df->get_data(std::size_t{3}, "m1") == 5.5;
	check->*df->get_data(std::size_t{4}, "m1") == 88.0;

	check->*df->get_data(std::size_t{5}, "m1") == 2.0;
	check->*df->get_data(std::size_t{6}, "m1") == 3.5;
	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{7}, "m1")))
	    == "is nan"_is_true;

	check->*df->get_data(std::size_t{8}, "m1") == 10.25;
	check->*df->get_data(std::size_t{9}, "m1") == 4.25;
	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{10}, "m1")))
	    == "is nan"_is_true;
}

    | "sort measure" |
    [](interface *df = if_setup{{"d1"},
           {"m1"},
           {
               {{"dm0", 5.5}},
               {{"dm1", 2.0}},
               {{"dm2", 3.5}},
               {{"dm3", 10.25}},
               {{"dm4", 88.0}},
               {{"dm5", 2.2}},
               {{"dm6", 7.4}},
               {{"dm7", NAN}},
               {{"dm8", 4.2}},
               {{"dm9", 0.0}},
           }})
{
	df->set_sort("m1", sort_type::greater, na_position::last);

	df->finalize();

	assert->*df->get_record_count() == std::size_t{10};

	check->*df->get_data(std::size_t{0}, "m1") == 88.0;
	check->*df->get_data(std::size_t{1}, "m1") == 10.25;
	check->*df->get_data(std::size_t{2}, "m1") == 7.4;
	check->*df->get_data(std::size_t{3}, "m1") == 5.5;
	check->*df->get_data(std::size_t{4}, "m1") == 4.2;
	check->*df->get_data(std::size_t{5}, "m1") == 3.5;
	check->*df->get_data(std::size_t{6}, "m1") == 2.2;
	check->*df->get_data(std::size_t{7}, "m1") == 2.0;
	check->*df->get_data(std::size_t{8}, "m1") == 0.0;
	check
	            ->*std::isnan(std::get<double>(
	                df->get_data(std::size_t{9}, "m1")))
	    == "is nan"_is_true;
}

;
